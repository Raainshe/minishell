/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:00:36 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/13 13:11:56 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Recursively processes all heredocs in the AST before execution
 *
 * This function traverses the AST and identifies all heredoc nodes.
 * It processes them sequentially to avoid race conditions when reading
 * from the terminal.
 *
 * @param node The root node of the AST or subtree
 * @param env The environment variables list
 * @return 0 on success, negative value on error (or -130 for SIGINT)
 */
int	preprocess_heredocs(t_node *node, t_list **env)
{
	return (traverse_tree_nodes(node, env));
}

/**
 * @brief Closes any lingering heredoc file descriptors
 *
 * This function is called after a signal interruption to ensure
 * all potential file descriptors are closed.
 *
 * @param count The number of potential file descriptors to check
 */
void	close_heredoc_fds(int count)
{
	int	i;
	int	start_fd;

	start_fd = 3;
	i = 0;
	while (i < count + 3)
	{
		close(start_fd + i);
		i++;
	}
}

/**
 * @brief Handles the heredoc input, reading until the delimiter is encountered
 * @param delimiter The delimiter string that ends the heredoc
 * @param env The environment variables list for expansion
 * @param expand_vars Whether to expand variables in the heredoc content
 * @return The file descriptor to read from, or -1 on error
 */
int	handle_heredoc(char *delimiter, t_list *env, bool expand_vars)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		fd_result;

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
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
		handle_heredoc_child(pipe_fd, delimiter, env, expand_vars);
	fd_result = handle_heredoc_parent(pipe_fd, pid);
	if (fd_result == -1 && pipe_fd[0] != -1)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (fd_result);
}

/**
 * @brief Processes a single heredoc node
 *
 * Checks if the heredoc has already been processed, and if not,
 * handles the heredoc input collection.
 *
 * @param node The heredoc node to process
 * @param env The environment variables list
 * @return 0 on success, negative value on error
 */
int	process_heredoc_node(t_node *node, t_list **env)
{
	t_redirect	*redirect;
	int			fd;

	redirect = (t_redirect *)node->data;
	if (redirect->heredoc_fd == -1)
	{
		fd = handle_heredoc(redirect->filename, *env, redirect->expand_vars);
		redirect->heredoc_fd = fd;
		if (fd < 0)
		{
			if (fd == -130)
				return (-130);
			return (-1);
		}
	}
	return (0);
}

/**
 * @brief Traverses the AST to find and process heredoc nodes
 *
 * @param node The current node being examined
 * @param env The environment variables list
 * @return 0 on success, negative value on error (or -130 for SIGINT)
 */
int	traverse_tree_nodes(t_node *node, t_list **env)
{
	int	result;

	if (!node)
		return (0);
	if (node->left)
	{
		result = traverse_tree_nodes(node->left, env);
		if (result < 0)
			return (result);
	}
	if (node->right)
	{
		result = traverse_tree_nodes(node->right, env);
		if (result < 0)
			return (result);
	}
	if (node->type == NODE_HERE_DOC)
		return (process_heredoc_node(node, env));
	return (0);
}
