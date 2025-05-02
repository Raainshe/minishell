/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_helper_helper_helper.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:45:28 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/25 14:16:42 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a character is a whitespace character
 * @param c The character to check
 * @return Non-zero if the character is whitespace, 0 otherwise
 */
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

/**
 * @brief Checks if a character is an operator (|, <, >)
 * @param c The character to check
 * @return true if the character is an operator, false otherwise
 */
bool	ft_is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/**
 * @brief Check if we have a multi-character operator (<< or >>) at position i
 * @param str The input string
 * @param i The position to check
 * @return true if a multi-character operator exists at position i, false
 * otherwise
 */
bool	ft_is_multi_char_op(char *str, int i)
{
	if (!str || !str[i] || !str[i + 1])
		return (false);
	return ((str[i] == '<' && str[i + 1] == '<') || (str[i] == '>' && str[i
				+ 1] == '>'));
}
