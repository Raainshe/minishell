/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:01:26 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/24 16:23:51 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Count the number of word tokens for the command
static int	count_command_args(t_parser_context *ctx)
{
	int		i;
	int		count;
	t_token	token;

	i = ctx->current_index;
	count = 0;
	token = ctx->tokens[i];
	while (token.type == TOKEN_WORD)
	{
		count++;
		i++;
		token = ctx->tokens[i];
		if (token.type == TOKEN_END)
			break ;
		if (token.type != TOKEN_WORD)
		{
			if ((token.type == TOKEN_REDIRECT_IN
					|| token.type == TOKEN_REDIRECT_OUT
					|| token.type == TOKEN_HERE_DOC
					|| token.type == TOKEN_APPEND) && ctx->tokens[i
					+ 1].type == TOKEN_WORD)
			{
				i += 2;
				token = ctx->tokens[i];
				continue ;
			}
			break ;
		}
	}
	return (count);
}

// Parse a simple command with its arguments
t_node	*parse_command(t_parser_context *ctx)
{
	t_token	token;
	char	**args;
	int		arg_count;
	int		i;
	t_node	*cmd_node;
	int		start_pos;

	// Save the starting position of the command
	start_pos = ctx->current_index;
	token = current_token(ctx);
	if (token.type != TOKEN_WORD)
	{
		parser_error(ctx, "Expected command");
		return (NULL);
	}
	arg_count = count_command_args(ctx);
	if (arg_count == 0)
		return (NULL);
	args = (char **)gc_malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	gc_add_context(AST, args);
	i = 0;
	// Reset to the beginning of the command
	ctx->current_index = start_pos;
	// Only collect WORD tokens for arguments, skip redirections
	while (i < arg_count)
	{
		token = current_token(ctx);
		if (token.type == TOKEN_WORD)
		{
			args[i] = token.content;
			next_token(ctx);
			i++;
		}
		else if (token.type >= TOKEN_REDIRECT_IN && token.type <= TOKEN_APPEND)
		{
			next_token(ctx);
			if (current_token(ctx).type == TOKEN_WORD)
				next_token(ctx);
		}
		else
			break ;
	}
	args[i] = NULL;
	cmd_node = create_command_node(args);
	if (!cmd_node)
		return (NULL);
	// Reset to the beginning of the command for redirection processing
	ctx->current_index = start_pos;
	return (handle_redirection(ctx, cmd_node));
}
