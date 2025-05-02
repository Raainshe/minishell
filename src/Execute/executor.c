/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:46:12 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/03 00:10:34 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Main execution function for the AST node
 *
 * Executes the appropriate action based on the node type.
 * Handles command nodes, pipe nodes, and various redirection nodes.
 *
 * @param node The AST node to execute
 * @param env Pointer to the environment variables list
 * @return Exit status of the executed command
 */
int	execute_node(t_node *node, t_list **env)
{
	int	status;

	status = 0;
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		status = execute_command(node, env);
	else if (node->type == NODE_PIPE)
		status = execute_pipe(node, env);
	else if (node->type == NODE_REDIRECT_IN || node->type == NODE_REDIRECT_OUT
		|| node->type == NODE_HERE_DOC || node->type == NODE_APPEND)
		status = execute_redirect(node, env);
	return (status);
}
