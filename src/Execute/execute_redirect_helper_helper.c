/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect_helper_helper.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:39:37 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/13 12:40:21 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Cleans up resources and exits the process.
 *
 * Closes file descriptors and exits with the specified code.
 *
 * @param pipe_fd Pipe file descriptor to close.
 * @param tty_fd Terminal file descriptor to close.
 * @param exit_code Exit code to use.
 */
void	clean_and_exit(int pipe_fd, int tty_fd, int exit_code)
{
	if (pipe_fd != -1)
		close(pipe_fd);
	if (tty_fd != -1)
		close(tty_fd);
	exit(exit_code);
}

/**
 * @brief Sets up the signal handler for SIGQUIT during heredoc.
 *
 * Configures SIGQUIT to be ignored while the heredoc is being processed.
 *
 * @param sa_orig_quit Pointer to store the original sigaction.
 * @param sa_ign_quit Pointer to the sigaction that ignores SIGQUIT.
 * @param pipe_fd Pipe file descriptors for cleanup on error.
 * @return 0 on success, -1 on error.
 */
int	setup_heredoc_signal(struct sigaction *sa_orig_quit,
		struct sigaction *sa_ign_quit, int pipe_fd[2])
{
	memset(sa_orig_quit, 0, sizeof(struct sigaction));
	memset(sa_ign_quit, 0, sizeof(struct sigaction));
	sa_ign_quit->sa_handler = SIG_IGN;
	sigemptyset(&sa_ign_quit->sa_mask);
	sa_ign_quit->sa_flags = 0;
	if (sigaction(SIGQUIT, sa_ign_quit, sa_orig_quit) == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	return (0);
}

/**
 * @brief Waits for the heredoc child process to complete.
 *
 * @param pid PID of the child process.
 * @param status Pointer to store the exit status.
 * @param pipe_fd Read end of pipe.
 * @param sa_orig_quit Original signal handler to restore on error.
 * @return 0 on success, -1 on error.
 */
int	wait_for_heredoc_child(pid_t pid, int *status, int pipe_fd,
		struct sigaction *sa_orig_quit)
{
	if (waitpid(pid, status, 0) == -1)
	{
		close(pipe_fd);
		sigaction(SIGQUIT, sa_orig_quit, NULL);
		return (-1);
	}
	return (0);
}

/**
 * @brief Restores the original SIGQUIT handler.
 *
 * @param sa_orig_quit Original signal handler to restore.
 */
void	restore_heredoc_signal(struct sigaction *sa_orig_quit)
{
	sigaction(SIGQUIT, sa_orig_quit, NULL);
}

/**
 * @brief Processes the exit status of the heredoc child.
 *
 * Handles various exit scenarios and sets global signal status if needed.
 *
 * @param status Exit status of the child.
 * @param pipe_fd Read end of pipe.
 * @return Pipe file descriptor on success, -1 on error.
 */
int	process_heredoc_status(int status, int pipe_fd)
{
	if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0))
	{
		close(pipe_fd);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			g_signal_received = SIGINT;
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
			g_signal_received = SIGINT;
		return (-1);
	}
	return (pipe_fd);
}
