/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:44:43 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/18 16:37:15 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes a parser context structure
 * @param context The context structure to initialize
 * @param tokens The array of tokens to parse
 * @return None
 * 
 * Sets up the parser context with the tokens to parse and initializes
 * the current index to 0 and error state to 0 (no error).
 */
void	init_parser_context(t_parser_context *context, t_token *tokens)
{
	if (!context || !tokens)
		return ;
	context->tokens = tokens;
	context->current_index = 0;
	context->error = 0;
	context->error_msg = NULL;
}

/**
 * @brief Returns the current token being parsed
 * @param context The parser context
 * @return The current token, or an END token if context is invalid
 * 
 * Gets the token at the current_index position in the token array.
 * If the context or token array is NULL, returns an empty END token.
 */
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

/**
 * @brief Returns the next token without advancing the current position
 * @param context The parser context
 * @return The next token, or an END token if at the end or context is invalid
 * 
 * Looks ahead to the next token without changing the current_index.
 * If the end of tokens is reached or the context is invalid, returns
 * an empty END token.
 */
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

/**
 * @brief Advances to the next token and returns the current one
 * @param context The parser context
 * @return The current token before advancing
 * 
 * Gets the current token and then increments the current_index to
 * move to the next token. If the current token is END, does not advance.
 */
t_token	next_token(t_parser_context *context)
{
	t_token	token;

	token = current_token(context);
	if (token.type != TOKEN_END)
		context->current_index++;
	return (token);
}

/**
 * @brief Records an error in the parser context
 * @param context The parser context
 * @param msg The error message to store
 * @return None
 * 
 * Sets the error flag and stores the error message in the context.
 * The message is allocated and added to the AST context for garbage collection.
 */
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
