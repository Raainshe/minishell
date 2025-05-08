/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:47:23 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/08 16:11:03 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute a redirection node in the AST
 *
	* This function processes a redirection node from the Abstract Syntax Tree
	(AST)
 * and executes the appropriate redirection operation based on the node type.
 * It supports input redirection (<), output redirection (>),
	output appending (>>),
 * and heredoc (<<) operations. The function delegates to specialized handlers
 * for each type of redirection.
 *
 * @param node The redirection node to execute, containing the type and data for
 *             the redirection operation

	* @param env A pointer to the environment variables list used for variable
				expansion
 * @return The exit status of the command after redirection, or an error code if
 *         the redirection fails
 */
int	execute_redirect(t_node *node, t_list **env)
{
	if (!node || !node->data || !node->left)
		return (1);
	if (node->type == NODE_REDIRECT_IN)
		return (handle_redirect_in(node, env));
	else if (node->type == NODE_REDIRECT_OUT)
		return (handle_redirect_out(node, env));
	else if (node->type == NODE_APPEND)
		return (handle_redirect_out(node, env));
	else if (node->type == NODE_HERE_DOC)
		return (handle_here_doc(node, env));
	return (1);
}
