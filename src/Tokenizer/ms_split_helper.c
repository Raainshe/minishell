/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:53:15 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/25 14:14:50 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates the quote state based on the current character
 * @param c The character being processed
 * @param in_quote Pointer to boolean flag for single quote state
 * @param in_double_quote Pointer to boolean flag for double quote state
 * @return None
 */
void	ft_update_quote_state(char c, bool *in_quote, bool *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_quote = !(*in_quote);
	else if (c == '\"' && !(*in_quote))
		*in_double_quote = !(*in_double_quote);
}

/**
 * @brief This is a continuiation of process_token_state
 * @return None
 */
void	ft_process_token_state_two(int *count, t_token_info *info, int *i)
{
	(*count)++;
	if (ft_is_multi_char_op(info->input, *i))
		(*i)++;
}

/**
 * @brief Processes the token state for the current character
 * @param info The token information structure
 * @param i The current position in the input
 * @param count Pointer to the token count
 * @return None
 */
static void	ft_process_token_state(t_token_info *info, int i, int *count)
{
	char	current;

	current = info->input[i];
	if (!(info->in_token) && !ft_isspace(current))
	{
		info->in_token = true;
		(*count)++;
	}
	if (info->in_token && (ft_isspace(current) || (ft_is_operator(current)
				&& !info->in_quote && !info->in_double_quote)))
	{
		info->in_token = false;
		if (ft_is_operator(current) && !info->in_quote
			&& !info->in_double_quote)
		{
			(*count)++;
			if (ft_is_multi_char_op(info->input, i))
				i++;
		}
	}
	else if (ft_is_operator(current) && !info->in_quote
		&& !info->in_double_quote && !(info->in_token))
	{
		ft_process_token_state_two(count, info, &i);
	}
}

/**
 * @brief Counts the number of tokens in a string
 * @param str The string to count tokens in
 * @return The number of tokens, or 0 if str is NULL
 */
int	ft_count_tokens(char *str)
{
	int				count;
	int				i;
	t_token_info	info;

	if (!str)
		return (0);
	count = 0;
	i = 0;
	ft_init_token_info(&info, str, 0);
	while (str[i])
	{
		ft_update_quote_state(str[i], &info.in_quote, &info.in_double_quote);
		ft_process_token_state(&info, i, &count);
		if (!info.in_quote && !info.in_double_quote && ft_is_multi_char_op(str,
				i))
			i++;
		i++;
	}
	return (count);
}

/**
 * @brief Initializes a token_info structure with the given parameters
 * @param info The token_info structure to initialize
 * @param str The input string to tokenize
 * @param token_count The number of tokens expected
 * @return None
 */
void	ft_init_token_info(t_token_info *info, char *str, int token_count)
{
	if (!info || !str)
		return ;
	info->input = str;
	info->count = token_count;
	info->i = 0;
	info->j = 0;
	info->in_quote = false;
	info->in_double_quote = false;
	info->in_token = false;
}
