/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:15:31 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/13 12:58:24 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Sets up the terminal for heredoc input.
 *
 * Gets the terminal name and opens it for reading.
 *
 * @return Terminal file descriptor on success, -1 on failure.
 */
int	setup_heredoc_terminal(void)
{
	char	*term_name;
	int		tty_fd;

	term_name = ttyname(STDOUT_FILENO);
	if (!term_name)
	{
		perror("minishell: ttyname failed for heredoc");
		return (-1);
	}
	tty_fd = open(term_name, O_RDONLY);
	if (tty_fd == -1)
	{
		perror("minishell: failed to open terminal for heredoc");
		return (-1);
	}
	return (tty_fd);
}

/**
 * @brief Redirects standard input to the terminal.
 *
 * Duplicates stdin and redirects it to the terminal file descriptor.
 *
 * @param tty_fd Terminal file descriptor.
 * @return Saved stdin file descriptor on success, -1 on failure.
 */
int	redirect_stdin_to_tty(int tty_fd)
{
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
	{
		perror("minishell: dup failed for heredoc saved_stdin");
		return (-1);
	}
	if (dup2(tty_fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 failed for heredoc tty->stdin");
		close(saved_stdin);
		return (-1);
	}
	return (saved_stdin);
}

/**
 * @brief Restores standard input from saved file descriptor.
 *
 * @param saved_stdin Saved stdin file descriptor.
 */
void	restore_stdin(int saved_stdin)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		perror("minishell: dup2 failed for heredoc saved->stdin");
	close(saved_stdin);
}

/**
 * @brief Processes a line from heredoc input.
 *
 * Checks if the line matches the delimiter. If not, expands variables
 * if necessary and writes to the pipe.
 *
 * @param line Input line to process.
 * @param delimiter Heredoc delimiter.
 * @param env Environment variables list.
 * @param expand_vars Whether to expand variables.
 * @param pipe_fd Pipe file descriptor to write to.
 * @param tty_fd Terminal file descriptor for cleanup.
 */
void	process_heredoc_line(t_process_heredoc_line *phl)
{
	if (ft_strlen(phl->line) == ft_strlen(phl->delimiter)
		&& ft_strncmp(phl->delimiter, phl->line,
			ft_strlen(phl->delimiter)) == 0)
	{
		free(phl->line);
		clean_and_exit(phl->pipe_fd, phl->tty_fd, 0);
	}
	if (phl->expand_vars && ft_strchr(phl->line, '$'))
		phl->line = expand_variables(phl->line, phl->env);
	ft_putendl_fd(phl->line, phl->pipe_fd);
	free(phl->line);
}

/**
 * @brief Manages the parent process during heredoc.
 *
 * Ignores `SIGQUIT` while waiting for the child. Waits for child (PID `pid`)
 * to finish heredoc input. Restores `SIGQUIT` handler.
 * If child is signaled (esp. `SIGINT`) or exits non-zero, sets
 * `g_signal_received` and returns -1.
 * On successful child completion, returns read end of pipe `pipe_fd[0]`.
 *
 * @param pipe_fd Pipe FDs. Parent reads from `pipe_fd[0]`.
 * @param pid PID of the child process handling heredoc.
 * @return Read FD of pipe on success, -1 on error/signal.
 */
int	handle_heredoc_parent(int pipe_fd[2], pid_t pid)
{
	int					status;
	struct sigaction	sa_orig_quit;
	struct sigaction	sa_ign_quit;

	status = 0;
	if (setup_heredoc_signal(&sa_orig_quit, &sa_ign_quit, pipe_fd) == -1)
		return (-1);
	close(pipe_fd[1]);
	if (wait_for_heredoc_child(pid, &status, pipe_fd[0], &sa_orig_quit) == -1)
		return (-1);
	restore_heredoc_signal(&sa_orig_quit);
	return (process_heredoc_status(status, pipe_fd[0]));
}
