/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_helper_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:33:32 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/25 14:05:17 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the start of a token in the input string
 * @param info The token information structure
 * @return true if we're in a token, false otherwise
 */
bool	ft_handle_token_start(t_token_info *info)
{
	char	current;

	current = info->input[info->i];
	if (!info->in_token && !ft_isspace(current))
	{
		info->token_start = info->i;
		info->in_token = true;
	}
	return (info->in_token);
}

/**
 * @brief Handles the end of a token in the input string
 * @param info The token information structure
 * @return false on error, true otherwise
 */
bool	ft_handle_token_end(t_token_info *info)
{
	char	current;
	char	next;

	current = info->input[info->i];
	next = info->input[info->i + 1];
	if (!info->in_token || info->in_quote || info->in_double_quote)
		return (true);
	if (ft_isspace(current))
	{
		if (!ft_add_token(info, info->token_start, info->i))
			return (false);
		info->in_token = false;
		return (true);
	}
	if (!ft_handle_operator(info, current, next))
		return (false);
	if (current != '<' && current != '>' && !ft_handle_next_operator(info,
			next))
		return (false);
	return (true);
}

/**
 * @brief Handles operators in the token parsing process
 * @param info The token information structure
 * @param current The current character being processed
 * @param next The next character in the input
 * @return false on error, true otherwise
 */
bool	ft_handle_operator(t_token_info *info, char current, char next)
{
	if (ft_handle_multi_char_op(info, current, next))
		return (true);
	if (ft_is_operator(current))
	{
		if (info->token_start < info->i)
		{
			if (!ft_add_token(info, info->token_start, info->i))
				return (false);
			info->token_start = info->i;
		}
		if (!ft_add_token(info, info->token_start, info->i + 1))
			return (false);
		info->in_token = false;
	}
	return (true);
}

/**
 * @brief Checks if the next character is an operator and handles it
 * @param info The token information structure
 * @param next The next character in the input
 * @return false on error, true otherwise
 */
bool	ft_handle_next_operator(t_token_info *info, char next)
{
	if (next && ft_is_operator(next) && !info->in_quote
		&& !info->in_double_quote)
	{
		if (info->in_token)
		{
			if (!ft_add_token(info, info->token_start, info->i + 1))
				return (false);
			info->in_token = false;
		}
	}
	return (true);
}

/**
 * @brief Handles multi-character operators (<< and >>) in the tokenization
 * process
 * @param info The token information structure
 * @param current The current character being processed
 * @param next The next character in the input
 * @return true if handled, false if not a multi-char operator or on error
 */
bool	ft_handle_multi_char_op(t_token_info *info, char current, char next)
{
	if ((current == '<' && next == '<') || (current == '>' && next == '>'))
	{
		if (info->token_start < info->i)
		{
			if (!ft_add_token(info, info->token_start, info->i))
				return (false);
			info->token_start = info->i;
		}
		if (!ft_add_token(info, info->token_start, info->i + 2))
			return (false);
		info->in_token = false;
		info->i++;
		return (true);
	}
	return (false);
}
