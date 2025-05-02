/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:29:16 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/24 16:35:07 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a generic AST node
 * @param type The type of node to create (command, pipe, redirection, etc.)
 * @param data The node-specific data (command args or redirection filename)
 * @param left The left child node
 * @param right The right child node
 * @return A new node with the specified attributes, or NULL if allocation fails
 *
 * This function allocates and initializes a new abstract syntax tree node
 * with the given type, data, and child nodes.
 */
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

/**
 * @brief Creates a command node with arguments
 * @param args The NULL-terminated array of command arguments
 * @return A new command node, or NULL if allocation fails
 *
 * This function creates a node of type NODE_COMMAND with the specified
 * command arguments. The first argument is treated as the command name.
 */
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

/**
 * @brief Creates a redirection node
 * @param type The type of redirection (in, out, here_doc, append)
 * @param filename The filename or delimiter for the redirection
 * @param command The command node to which this redirection applies
 * @return A new redirection node, or NULL if allocation fails
 *
 * This function creates a redirection node of the specified type
 * with the given filename, and sets the command as its left child.
 */
t_node	*create_redirect_node(t_node_type type, char *filename, t_node *command)
{
	t_node		*node;
	t_redirect	*redirect;

	redirect = (t_redirect *)gc_malloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	gc_add_context(AST, redirect);
	redirect->filename = filename;
	redirect->expand_vars = true;
	node = create_node(type, redirect, command, NULL);
	return (node);
}

/**
 * @brief Creates a pipe node connecting two commands
 * @param left The command node on the left side of the pipe
 * @param right The command node on the right side of the pipe
 * @return A new pipe node, or NULL if allocation fails
 *
 * This function creates a node of type NODE_PIPE with the left command
 * as its left child and the right command as its right child.
 */
t_node	*create_pipe_node(t_node *left, t_node *right)
{
	return (create_node(NODE_PIPE, NULL, left, right));
}
