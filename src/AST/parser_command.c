/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:01:26 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/25 15:31:28 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Helper function for counting commands in a loop
 * @param token Current token being examined
 * @param ctx Parser context
 * @param count Pointer to the count variable to increment
 * @param i Current index in the token array
 * @return Updated count of command arguments
 *
 * This function processes tokens in a loop to count command arguments,
 * handling redirections appropriately.
 */
static int	count_commands_loop(t_token token, t_parser_context *ctx,
		int *count, int i)
{
	while (token.type == TOKEN_WORD)
	{
		(*count)++;
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
	return (*count);
}

/**
 * @brief Counts the number of command arguments
 * @param ctx The parser context
 * @return The number of word tokens that make up the command and its arguments
 *
 * This function iterates through tokens starting at the current position
 * to count the number of arguments in a command, skipping over redirections
 * and their filenames.
 */
static int	count_command_args(t_parser_context *ctx)
{
	int		i;
	int		count;
	t_token	token;

	i = ctx->current_index;
	token = ctx->tokens[i];
	count = 0;
	return (count_commands_loop(token, ctx, &count, i));
}

/**
 * @brief Builds command arguments and creates a command node
 * @param ctx The parser context containing tokens
 * @param args Array to store command arguments
 * @param arg_count The number of arguments to process
 * @param start_pos The starting position in the token array
 * @return A command node containing the arguments or NULL if creation fails
 *
 * This function populates the args array with command arguments from tokens,
 * skipping over redirections and their targets. It then creates and returns
 * a command node with these arguments.
 */
static t_node	*build_command_args(t_parser_context *ctx, char **args,
		int arg_count, int start_pos)
{
	int		i;
	t_token	token;

	i = 0;
	ctx->current_index = start_pos;
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
	return (create_command_node(args));
}

/**
 * @brief Parses a simple command with its arguments
 * @param ctx The parser context containing tokens and state
 * @return A command node or NULL if parsing fails
 *
 * This function processes tokens to create a command node with arguments,
 * handling redirections through the handle_redirection function.
 * It first counts the arguments, allocates memory for them, builds the command,
 * and then processes any redirections.
 */
t_node	*parse_command(t_parser_context *ctx)
{
	t_token	token;
	char	**args;
	int		arg_count;
	int		start_pos;
	t_node	*cmd_node;

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
	cmd_node = build_command_args(ctx, args, arg_count, start_pos);
	if (!cmd_node)
		return (NULL);
	ctx->current_index = start_pos;
	return (handle_redirection(ctx, cmd_node));
}
