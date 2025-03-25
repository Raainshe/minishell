/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:00:02 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/25 15:11:20 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Adds a new token to the token array from substring of input
 * @param info The token information structure
 * @param start The starting index of the token in the input string
 * @param end The ending index (exclusive) of the token in the input string
 * @return false on error, true on success
 */
bool	ft_add_token(t_token_info *info, int start, int end)
{
	char	*token;

	token = ft_strndup(&info->input[start], end - start);
	if (!token)
		return (false);
	info->tokens[info->j] = token;
	info->j++;
	return (true);
}

/**
 * @brief Handles remaining tokens after the main tokenization loop
 * @param str The input string
 * @param info The token information structure
 * @return false on error, true on success
 */
static bool	ft_handle_remaining_tokens(char *str, t_token_info *info)
{
	int	remaining_start;

	remaining_start = info->i;
	while (str[remaining_start] && ft_isspace(str[remaining_start]))
		remaining_start++;
	if (str[remaining_start])
	{
		info->i = ft_strlen(str);
		return (ft_add_token(info, remaining_start, info->i));
	}
	return (true);
}

/**
 * @brief Processes the final token in the input string
 * @param info The token information structure
 * @return false on error, true on success
 */
static bool	ft_process_final_token(t_token_info *info)
{
	char	*token;

	if (info->in_token && info->j < info->count)
	{
		token = ft_strndup(&info->input[info->token_start], info->i
				- info->token_start);
		if (!token)
			return (false);
		info->tokens[info->j] = token;
		info->j++;
	}
	if (info->input[info->i] && info->j < info->count)
	{
		if (!ft_handle_remaining_tokens(info->input, info))
			return (false);
	}
	info->tokens[info->j] = NULL;
	return (true);
}

/**
 * @brief Processes all tokens in the input string
 * @param str The input string to tokenize
 * @param tokens The token array to fill
 * @param token_count The maximum number of tokens to extract
 * @return The token array or NULL on error
 */
static char	**ft_process_tokens(char *str, char **tokens, int token_count)
{
	t_token_info	info;

	if (!str || !tokens)
		return (NULL);
	ft_init_token_info(&info, str, token_count);
	info.tokens = tokens;
	while (str[info.i] && info.j < token_count)
	{
		ft_update_quote_state(str[info.i], &info.in_quote,
			&info.in_double_quote);
		ft_handle_token_start(&info);
		if (!ft_handle_token_end(&info))
			return (NULL);
		info.i++;
	}
	if (!ft_process_final_token(&info))
		return (NULL);
	return (tokens);
}

/**
 * @brief Splits a string into tokens according to shell parsing rules
 * @param str The input string to tokenize
 * @return An array of token strings or NULL on error
 */
char	**ft_split_tokens(char *str)
{
	char	**tokens;
	int		token_count;

	if (!str)
		return (NULL);
	token_count = ft_count_tokens(str);
	token_count += 1;
	tokens = (char **)gc_malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	gc_add_context(TOKENIZER, tokens);
	return (ft_process_tokens(str, tokens, token_count));
}
