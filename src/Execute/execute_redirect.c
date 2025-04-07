/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:47:23 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/07 15:11:35 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute a redirection node in the AST
 * @param node The redirection node to execute
 * @param env The environment variables
 * @return The exit status of the command
 */
int	execute_redirect(t_node *node, t_list *env)
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
		ft_putendl_fd("minishell: here-document not implemented yet",
			STDERR_FILENO);
		status = 1;
	}
	else
		status = 1;
	return (status);
}
