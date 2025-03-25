/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:07:28 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/25 14:32:00 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Process a single redirection
static t_node	*process_redirection(t_parser_context *ctx, t_node *cmd_node)
{
	t_token		current;
	t_token		next_tok;
	t_node_type	redirect_type;
	char		*filename;

	current = current_token(ctx);
	if (current.type == TOKEN_REDIRECT_IN)
		redirect_type = NODE_REDIRECT_IN;
	else if (current.type == TOKEN_REDIRECT_OUT)
		redirect_type = NODE_REDIRECT_OUT;
	else if (current.type == TOKEN_HERE_DOC)
		redirect_type = NODE_HERE_DOC;
	else if (current.type == TOKEN_APPEND)
		redirect_type = NODE_APPEND;
	else
		return (cmd_node);
	next_token(ctx);
	next_tok = current_token(ctx);
	if (next_tok.type != TOKEN_WORD)
		return (parser_error(ctx, "Expected filename after redirection"), NULL);
	filename = next_tok.content;
	next_token(ctx);
	return (create_redirect_node(redirect_type, filename, cmd_node));
}

// Handle all redirections for a command
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
