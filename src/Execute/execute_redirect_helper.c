/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:15:31 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/12 16:59:53 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>  // Needed for O_RDONLY
#include <unistd.h> // Needed for ttyname, isatty? Included via minishell.h?

/**
 * @brief Prints an error message for redirection failures.
 *
 * Displays a formatted error to `STDERR_FILENO`:
 * "minishell: [filename]: [error_description]".
 * Used when a file redirection operation fails.
 *
 * @param redirect Ptr to `t_redirect` with the problematic `filename`.
 * @return Always returns 1 (error).
 */
int	print_redirect_error(t_redirect *redirect)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(redirect->filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	return (1);
}

/**
 * @brief Handles heredoc input in the child process.
 *
 * Sets heredoc signal handlers. Reads lines using `readline` until `delimiter`
 * is matched or EOF (Ctrl+D) is received. Input lines are written to `pipe_fd
 * [1]`.
 * Variables are expanded if `expand_vars` is true.
 * Exits 0 on success, 130 on EOF/Ctrl+D.
 *
 * @param pipe_fd Pipe FDs. Child writes to `pipe_fd[1]`.
 * @param delimiter String ending heredoc.
 * @param env Environment variables for expansion.
 * @param expand_vars If true, expand variables in heredoc.
 */
void	handle_heredoc_child(int pipe_fd[2], char *delimiter, t_list *env,
		bool expand_vars)
{
	char	*line;
	int		tty_fd;
	int		saved_stdin;
	char	*term_name;

	tty_fd = -1;
	saved_stdin = -1;
	term_name = NULL;
	setup_heredoc_signals();
	close(pipe_fd[0]);
	term_name = ttyname(STDOUT_FILENO);
	if (!term_name)
	{
		perror("minishell: ttyname failed for heredoc");
		close(pipe_fd[1]);
		exit(1);
	}
	tty_fd = open(term_name, O_RDONLY);
	if (tty_fd == -1)
	{
		perror("minishell: failed to open terminal for heredoc");
		close(pipe_fd[1]);
		exit(1);
	}
	while (1)
	{
		saved_stdin = dup(STDIN_FILENO);
		if (saved_stdin == -1)
		{
			perror("minishell: dup failed for heredoc saved_stdin");
			close(tty_fd);
			close(pipe_fd[1]);
			exit(1);
		}
		if (dup2(tty_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 failed for heredoc tty->stdin");
			close(saved_stdin);
			close(tty_fd);
			close(pipe_fd[1]);
			exit(1);
		}
		line = readline("> ");
		if (dup2(saved_stdin, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 failed for heredoc saved->stdin");
			close(saved_stdin);
			close(tty_fd);
			close(pipe_fd[1]);
			exit(1);
		}
		close(saved_stdin);
		saved_stdin = -1;
		if (!line)
			break ;
		if (ft_strlen(line) == ft_strlen(delimiter) && ft_strncmp(delimiter,
				line, ft_strlen(delimiter)) == 0)
		{
			free(line);
			close(pipe_fd[1]);
			close(tty_fd);
			exit(0);
		}
		if (expand_vars && ft_strchr(line, '$'))
			line = expand_variables(line, env);
		ft_putendl_fd(line, pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	close(tty_fd);
	exit(130);
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
	memset(&sa_orig_quit, 0, sizeof(struct sigaction));
	memset(&sa_ign_quit, 0, sizeof(struct sigaction));
	sa_ign_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_ign_quit.sa_mask);
	sa_ign_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_ign_quit, &sa_orig_quit) == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	close(pipe_fd[1]);
	if (waitpid(pid, &status, 0) == -1)
	{
		close(pipe_fd[0]);
		sigaction(SIGQUIT, &sa_orig_quit, NULL);
		return (-1);
	}
	sigaction(SIGQUIT, &sa_orig_quit, NULL);
	if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0))
	{
		close(pipe_fd[0]);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			g_signal_received = SIGINT;
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
			g_signal_received = SIGINT;
		return (-1);
	}
	return (pipe_fd[0]);
}
