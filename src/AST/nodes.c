/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:29:16 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/24 15:04:21 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_node(t_node_type type, void *data, t_node *left, t_node *right)
{
	t_node	*node;

	node = (t_node *)gc_malloc(sizeof(t_node));
	if (!node)
	{
		free_gc();
		return (NULL);
	}
	gc_add_context(AST, node);
	node->type = type;
	node->data = data;
	node->left = left;
	node->right = right;
	return (node);
}

t_node	*create_command_node(char **args)
{
	t_node		*node;
	t_command	*command;

	command = (t_command *)gc_malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	gc_add_context(AST, command);
	command->args = args;
	command->redirects = NULL;
	node = create_node(NODE_COMMAND, command, NULL, NULL);
	return (node);
}

t_node	*create_redirect_node(t_node_type type, char *filename, t_node *command)
{
	t_node		*node;
	t_redirect	*redirect;

	redirect = (t_redirect *)gc_malloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	gc_add_context(AST, redirect);
	redirect->filename = filename;
	node = create_node(type, redirect, command, NULL);
	return (node);
}

t_node	*create_pipe_node(t_node *left, t_node *right)
{
	return (create_node(NODE_PIPE, NULL, left, right));
}
