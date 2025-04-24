/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:47:04 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/24 13:44:15 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes a pipeline of two commands connected by a pipe.
 *
 * This function takes a pipe node from the Abstract Syntax Tree (AST) and
 * executes the commands represented by its left and right child nodes.
 * The left node's output is piped as input to the right node. The function
 * handles the creation of the pipe, forking of child processes, and signal
 * management. It waits for both child processes to complete and returns
 * the exit status of the last command in the pipeline.
 *
 * @param node The pipe node containing the commands to execute. Must have
 *             both left and right child nodes.
 * @param env A pointer to the environment variables list.
 * @return The exit status of the last command in the pipeline, or an error
 *         code if the execution fails.
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
	{
		perror("minishell: pipe");
		return (1);
	}
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("minishell: fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (pid1 == 0)
	{
		reset_signals();
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execute_node(node->left, env));
	}
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("minishell: fork");
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(pid1, NULL, 0);
		return (1);
	}
	if (pid2 == 0)
	{
		reset_signals();
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(execute_node(node->right, env));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
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
