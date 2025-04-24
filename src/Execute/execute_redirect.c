/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:47:23 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/24 15:15:22 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the heredoc input, reading until the delimiter is encountered
 * @param delimiter The delimiter string that ends the heredoc
 * @param env The environment variables list for expansion
 * @return The file descriptor to read from, or -1 on error
 */
static int	handle_heredoc(char *delimiter, t_list *env)
{
	int		pipe_fd[2];
	pid_t	pid;
	char	*line;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		setup_heredoc_signals();
		close(pipe_fd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
			{
				free(line);
				close(pipe_fd[1]);
				exit(0);
			}
			// Expand variables in the heredoc content
			if (ft_strchr(line, '$'))
				line = expand_variables(line, env);
			ft_putendl_fd(line, pipe_fd[1]);
			free(line);
		}
	}
	else
	{
		close(pipe_fd[1]);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) || (WIFEXITED(status)
				&& WEXITSTATUS(status) != 0))
		{
			close(pipe_fd[0]);
			return (-1);
		}
	}
	return (pipe_fd[0]);
}

/**
 * @brief Execute a redirection node in the AST
 * @param node The redirection node to execute
 * @param env The environment variables
 * @return The exit status of the command
 */
int	execute_redirect(t_node *node, t_list **env)
{
	int			fd;
	int			saved_fd;
	int			status;
	t_redirect	*redirect;

	if (!node || !node->data || !node->left)
		return (1);
	redirect = (t_redirect *)node->data;
	if (node->type == NODE_REDIRECT_IN)
	{
		fd = open(redirect->filename, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(redirect->filename, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			return (1);
		}
		saved_fd = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
		status = execute_node(node->left, env);
		dup2(saved_fd, STDIN_FILENO);
		close(saved_fd);
	}
	else if (node->type == NODE_REDIRECT_OUT)
	{
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(redirect->filename, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			return (1);
		}
		saved_fd = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		status = execute_node(node->left, env);
		dup2(saved_fd, STDOUT_FILENO);
		close(saved_fd);
	}
	else if (node->type == NODE_APPEND)
	{
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(redirect->filename, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			return (1);
		}
		saved_fd = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		status = execute_node(node->left, env);
		dup2(saved_fd, STDOUT_FILENO);
		close(saved_fd);
	}
	else if (node->type == NODE_HERE_DOC)
	{
		fd = handle_heredoc(redirect->filename, *env);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: error in heredoc\n", STDERR_FILENO);
			return (1);
		}
		saved_fd = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
		status = execute_node(node->left, env);
		dup2(saved_fd, STDIN_FILENO);
		close(saved_fd);
	}
	else
		status = 1;
	return (status);
}
