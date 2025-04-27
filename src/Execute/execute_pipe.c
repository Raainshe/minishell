/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:47:04 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/27 10:58:01 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes the left (first) command in a pipe
 *
 * Sets up stdout redirection to pipe and executes the command.
 *
 * @param node Pipe node containing command to execute
 * @param pipefd Pipe file descriptors
 * @param env Environment variables list
 */
static void	execute_left_pipe(t_node *node, int *pipefd, t_list **env)
{
	reset_signals();
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_node(node->left, env));
}

/**
 * @brief Executes the right (second) command in a pipe
 *
 * Sets up stdin redirection from pipe and executes the command.
 *
 * @param node Pipe node containing command to execute
 * @param pipefd Pipe file descriptors
 * @param env Environment variables list
 */
static void	execute_right_pipe(t_node *node, int *pipefd, t_list **env)
{
	reset_signals();
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute_node(node->right, env));
}

/**
 * @brief Processes the exit status of child processes
 *
 * Handles the exit status and signal information from child processes
 *
 * @param status Process status from waitpid
 * @return Appropriate exit status
 */
static int	process_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			g_signal_received = SIGINT;
		else if (WTERMSIG(status) == SIGQUIT)
			g_signal_received = SIGQUIT;
		return (128 + WTERMSIG(status));
	}
	return (1);
}

/**
 * @brief Handles fork error in pipe execution
 *
 * Prints error message, closes pipe file descriptors,
 * and cleans up any running child processes.
 *
 * @param pipefd Pipe file descriptors to close
 * @param pid Already forked process to clean up (0 if none)
 * @return Always returns 1 (error code for fork failure)
 */
static int	handle_fork_error(int *pipefd, pid_t pid)
{
	perror("minishell: fork");
	close(pipefd[0]);
	close(pipefd[1]);
	if (pid > 0)
		waitpid(pid, NULL, 0);
	return (1);
}

/**
 * @brief Executes a pipeline of two commands connected by a pipe.
 *
 * This function takes a pipe node from the Abstract Syntax Tree (AST) and
 * executes the commands represented by its left and right child nodes.
 *
 * @param node The pipe node containing the commands to execute
 * @param env A pointer to the environment variables list
 * @return The exit status of the last command in the pipeline
 */
int	execute_pipe(t_node *node, t_list **env)
{
	int		pipefd[2];
	int		status;
	pid_t	pid1;
	pid_t	pid2;

	if (!node || !node->left || !node->right)
		return (1);
	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), 1);
	pid1 = fork();
	if (pid1 < 0)
		return (handle_fork_error(pipefd, 0));
	if (pid1 == 0)
		execute_left_pipe(node, pipefd, env);
	pid2 = fork();
	if (pid2 < 0)
		return (handle_fork_error(pipefd, pid1));
	if (pid2 == 0)
		execute_right_pipe(node, pipefd, env);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	return (process_exit_status(status));
}
