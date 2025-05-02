/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:47:23 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 11:21:03 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Main entry point for parsing tokens into an abstract syntax tree
 * @param tokens Array of tokens to parse
 * @return Root node of the abstract syntax tree, or NULL on error
 *
 * This function initializes the parser context and starts the parsing process
 * by calling parse_pipeline. It handles any errors that occur during parsing.
 */
t_node	*parse_tokens(t_token *tokens)
{
	t_parser_context	context;
	t_node				*ast;
	int					*exit_code;

	if (!tokens)
		return (NULL);
	init_parser_context(&context, tokens);
	exit_code = ft_exit_code_holder();
	context.exit_code = exit_code;
	ast = parse_pipeline(&context);
	if (context.error)
	{
		if (context.error_msg)
			ft_putendl_fd(context.error_msg, STDERR_FILENO);
		return (NULL);
	}
	return (ast);
}

/**
 * @brief Parses a pipeline of one or more commands connected with pipes
 * @param context The parser context containing the tokens and state
 * @return Root node of the pipeline, or NULL on error
 *
 * This function parses a sequence of commands connected by pipe symbols (|).
 * It builds a binary tree where each pipe node has the left command as its
 * left child and the right command (or pipeline) as its right child.
 */
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
