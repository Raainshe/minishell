/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:07:28 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/18 16:12:58 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Reset parser position to start of command to process redirections
static void	reset_for_redirections(t_parser_context *ctx, int original_pos)
{
	ctx->current_index = original_pos;
}

// Process a single redirection
static t_node	*process_redirection(t_parser_context *ctx, t_node *cmd_node)
{
	t_token		token;
	t_node_type	redirect_type;
	char		*filename;

	token = current_token(ctx);
	if (token.type == TOKEN_REDIRECT_IN)
		redirect_type = NODE_REDIRECT_IN;
	else if (token.type == TOKEN_REDIRECT_OUT)
		redirect_type = NODE_REDIRECT_OUT;
	else if (token.type == TOKEN_HERE_DOC)
		redirect_type = NODE_HERE_DOC;
	else if (token.type == TOKEN_APPEND)
		redirect_type = NODE_APPEND;
	else
		return (cmd_node);
	token = current_token(ctx);
	if (token.type != TOKEN_WORD)
	{
		parser_error(ctx, "Expected filename after redirection");
		return (NULL);
	}
	filename = token.content;
	next_token(ctx);
	return (create_redirect_node(redirect_type, filename, cmd_node));
}

// Handle all redirections for a command
t_node	*handle_redirection(t_parser_context *ctx, t_node *cmd_node)
{
	int		original_pos;
	t_token	token;
	t_node	*result;

	if (!cmd_node)
		return (NULL);
	original_pos = ctx->current_index;
	while (original_pos > 0)
	{
		if (ctx->tokens[original_pos - 1].type == TOKEN_PIPE || original_pos
			- 1 == 0)
			break ;
		original_pos--;
	}
	reset_for_redirections(ctx, original_pos);
	while ((token = current_token(ctx)).type == TOKEN_WORD)
		next_token(ctx);
	result = cmd_node;
	while (1)
	{
		token = current_token(ctx);
		if (token.type >= TOKEN_REDIRECT_IN && token.type <= TOKEN_APPEND)
		{
			result = process_redirection(ctx, result);
			if (!result)
				return (NULL);
		}
		else if (token.type == TOKEN_WORD)
			next_token(ctx);
		else
			break ;
	}
	return (result);
}
