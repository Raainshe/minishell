/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:47:23 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/18 16:01:15 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_tokens(t_token *tokens)
{
	t_parser_context	context;
	t_node				*ast;

	if (!tokens)
		return (NULL);
	init_parser_context(&context, tokens);
	ast = parse_pipeline(&context);
	if (context.error)
	{
		if (context.error_msg)
			ft_putendl_fd(context.error_msg, STDERR_FILENO);
		return (NULL);
	}
	return (ast);
}

// TODO: Check correct error message
t_node	*parse_pipeline(t_parser_context *context)
{
	t_node	*left;
	t_node	*right;
	t_token	token;

	left = parse_command(context);
	if (!left || context->error)
		return (NULL);
	token = current_token(context);
	while (token.type == TOKEN_PIPE)
	{
		next_token(context);
		right = parse_command(context);
		if (!right)
		{
			parser_error(context, "Expected command after pipe");
			return (NULL);
		}
		left = create_pipe_node(left, right);
		if (!left)
			return (NULL);
		token = current_token(context);
	}
	return (left);
}
