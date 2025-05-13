/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:09:02 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/13 13:15:34 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes heredoc_fd field for all redirects in the AST
 *
 * This must be called before preprocessing to ensure all heredoc_fd
 * fields are set to -1 (unprocessed).
 *
 * @param node The root node of the AST or subtree
 * @return void
 */
void	init_heredoc_fds(t_node *node)
{
	t_redirect	*redirect;

	if (!node)
		return ;
	init_heredoc_fds(node->left);
	init_heredoc_fds(node->right);
	if (node->type == NODE_HERE_DOC && node->data)
	{
		redirect = (t_redirect *)node->data;
		redirect->heredoc_fd = -1;
	}
}

/**
 * @brief Closes all preprocessed heredoc file descriptors in the AST
 *
 * This should be called after pipeline execution to clean up resources.
 *
 * @param node The root node of the AST or subtree
 * @return void
 */
void	close_preprocessed_heredocs(t_node *node)
{
	t_redirect	*redirect;

	if (!node)
		return ;
	close_preprocessed_heredocs(node->left);
	close_preprocessed_heredocs(node->right);
	if (node->type == NODE_HERE_DOC && node->data)
	{
		redirect = (t_redirect *)node->data;
		if (redirect->heredoc_fd >= 0)
		{
			close(redirect->heredoc_fd);
			redirect->heredoc_fd = -1;
		}
	}
}

/**
 * @brief Collects all heredoc nodes into an array
 * @param node The starting node
 * @param heredoc_nodes Array to store the collected heredoc nodes
 * @return The command node after all heredoc nodes
 */
t_node	*collect_heredoc_nodes(t_node *node, t_node *heredoc_nodes[100],
		int *heredoc_count)
{
	t_node	*cmd_node;

	*heredoc_count = 0;
	cmd_node = node;
	while (cmd_node && cmd_node->type == NODE_HERE_DOC)
	{
		heredoc_nodes[(*heredoc_count)++] = cmd_node;
		cmd_node = cmd_node->left;
	}
	return (cmd_node);
}

/**
 * @brief Handles cleanup and error reporting when processing heredocs
 *
 * @param fd The current file descriptor
 * @param last_fd The previous file descriptor that needs to be closed
 * @return The appropriate error code
 */
int	handle_heredoc_error(int last_fd)
{
	if (last_fd != -1)
		close(last_fd);
	if (g_signal_received == SIGINT)
		return (-130);
	return (-1);
}
