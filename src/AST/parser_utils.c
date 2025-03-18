/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:44:43 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/18 15:17:35 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_parser_context(t_parser_context *context, t_token *tokens)
{
	if (!context || !tokens)
		return ;
	context->tokens = tokens;
	context->current_index = 0;
	context->error = 0;
	context->error_msg = NULL;
}

t_token	current_token(t_parser_context *context)
{
	t_token	empty;

	if (!context || !context->tokens)
	{
		empty.type = TOKEN_END;
		empty.content = NULL;
		return (empty);
	}
	return (context->tokens[context->current_index]);
}

t_token	peek_next_token(t_parser_context *context)
{
	t_token	empty;

	if (!context || !context->tokens
		|| context->tokens[context->current_index].type == TOKEN_END)
	{
		empty.type = TOKEN_END;
		empty.content = NULL;
		return (empty);
	}
	return (context->tokens[context->current_index + 1]);
}

t_token	next_token(t_parser_context *context)
{
	t_token	token;

	token = current_token(context);
	if (token.type != TOKEN_END)
		context->current_index++;
	return (token);
}

void	parser_error(t_parser_context *context, char *msg)
{
	char	*error_msg;

	context->error = 1;
	error_msg = ft_strdup(msg);
	if (!error_msg)
	{
		free_gc();
		return ;
	}
	context->error_msg = error_msg;
	gc_add_context(AST, context->error_msg);
}
