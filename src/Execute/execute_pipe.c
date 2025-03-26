/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:47:04 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/26 15:39:21 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute a pipe node in the AST
 * @param node The pipe node to execute
 * @param env The environment variables
 * @return The exit status of the last command in the pipeline
 */
int	execute_pipe(t_node *node, char **env)
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
		return (128 + WTERMSIG(status));
	return (1);
}
