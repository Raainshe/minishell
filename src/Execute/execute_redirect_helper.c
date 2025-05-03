/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:15:31 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/03 15:38:14 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Prints an error message for redirection failures
 *

	* This function prints a formatted error message when a redirection
	operation fails,

	* displaying the filename that caused the error along with the corresponding
		error
 * message from the system.
 *

	* @param redirect The redirection structure containing the filename that
	caused the error
 * @return Always returns 1 to indicate an error occurred
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
 * @brief Handles the child process for heredoc
 * @param pipe_fd The pipe file descriptors
 * @param delimiter The delimiter string that ends the heredoc
 * @param env The environment variables list for expansion
 * @param expand_vars Whether to expand variables in the heredoc
 */
void	handle_heredoc_child(int pipe_fd[2], char *delimiter, t_list *env,
		bool expand_vars)
{
	char	*line;

	setup_heredoc_signals();
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(pipe_fd[1]);
			exit(130);
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			close(pipe_fd[1]);
			exit(0);
		}
		if (expand_vars && ft_strchr(line, '$'))
			line = expand_variables(line, env);
		ft_putendl_fd(line, pipe_fd[1]);
		free(line);
	}
}

/**
 * @brief Handles the parent process for heredoc
 * @param pipe_fd The pipe file descriptors
 * @param pid The child process ID
 * @return The heredoc file descriptor, or -1 on error
 */
int	handle_heredoc_parent(int pipe_fd[2], pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0))
	{
		close(pipe_fd[0]);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			g_signal_received = SIGINT;
		}
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			g_signal_received = SIGINT;
		}
		return (-1);
	}
	return (pipe_fd[0]);
}
