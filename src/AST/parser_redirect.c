/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:07:28 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/29 14:15:09 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node_type	set_redirect_type(t_token current)
{
	if (current.type == TOKEN_REDIRECT_IN)
		return (NODE_REDIRECT_IN);
	else if (current.type == TOKEN_REDIRECT_OUT)
		return (NODE_REDIRECT_OUT);
	else if (current.type == TOKEN_HERE_DOC)
		return (NODE_HERE_DOC);
	else if (current.type == TOKEN_APPEND)
		return (NODE_APPEND);
	else
		return (NODE_NULL);
}

/**
 * @brief Processes a single redirection token and creates a redirection node
 * @param ctx The parser context
 * @param cmd_node The command node to attach the redirection to
 * @return A new redirection node with the command as its child,
	or NULL on error
 *
 * This function handles a single redirection operator (<, >, <<, >>)
 * and creates the appropriate redirection node with the filename that follows.
 */
static t_node	*process_redirection(t_parser_context *ctx, t_node *cmd_node)
{
	t_token		current;
	t_token		next_tok;
	t_node_type	redirect_type;
	char		*filename;
	t_node		*redirect_node;
	t_redirect	*redirect_data;

	current = current_token(ctx);
	redirect_type = set_redirect_type(current);
	if (redirect_type == NODE_NULL)
		return (cmd_node);
	next_token(ctx);
	next_tok = current_token(ctx);
	if (next_tok.type != TOKEN_WORD)
	{
		parser_error(ctx, "Expected filename after redirection");
		// Exit code 2 is set in parser_error
		return (NULL);
	}
	filename = next_tok.content;
	next_token(ctx);
	redirect_node = create_redirect_node(redirect_type, filename, cmd_node);
	if (redirect_type == NODE_HERE_DOC && redirect_node)
	{
		redirect_data = (t_redirect *)redirect_node->data;
		redirect_data->expand_vars = !next_tok.was_quoted;
	}
	return (redirect_node);
}

/**
 * @brief Handles all redirections associated with a command
 * @param ctx The parser context
 * @param cmd_node The command node to attach redirections to
 * @return The modified command node with redirections, or NULL on error
 *
 * This function processes all redirections that follow a command,
 * creating a chain of redirection nodes until it encounters a pipe
 * or the end of input.
 */
t_node	*handle_redirection(t_parser_context *ctx, t_node *cmd_node)
{
	t_token	token;
	t_node	*result;

	if (!cmd_node)
		return (NULL);
	result = cmd_node;
	while (1)
	{
		token = current_token(ctx);
		if (token.type == TOKEN_PIPE || token.type == TOKEN_END)
			break ;
		if (token.type >= TOKEN_REDIRECT_IN && token.type <= TOKEN_APPEND)
		{
			result = process_redirection(ctx, result);
			if (!result)
				return (NULL);
		}
		else
			next_token(ctx);
	}
	return (result);
}
