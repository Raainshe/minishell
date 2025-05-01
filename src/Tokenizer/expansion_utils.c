/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:19:20 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/01 11:24:18 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_quote_type(int *in_quotes, char *quote_type, char character,
		int *i)
{
	*in_quotes = 1;
	*quote_type = character;
	(*i)++;
}

void	update_quote_type_neg(int *in_quotes, char *quote_type, int *i)
{
	*in_quotes = 0;
	*quote_type = '\0';
	(*i)++;
}

/**
 * @brief Process variable or exit code expansion
 * @param str String being processed
 * @param env Environment list
 * @param result Current result string
 * @param i Pointer to current position in string
 * @return Updated result string or NULL on failure
 */
char	*process_var_expansion(char *str, t_list *env, char *result, int *i)
{
	char	*temp;

	if (str[*i + 1] == '?')
	{
		temp = process_exit_code(result, i);
		if (!temp)
			return (NULL);
		return (temp);
	}
	else
	{
		temp = process_variable(str, env, result, i);
		if (!temp)
			return (NULL);
		return (temp);
	}
}

/**
 * @brief Append a single character to result string with memory management
 * @param result Current result string
 * @param str Source string
 * @param i Pointer to current position in string
 * @return Updated result string or NULL on failure
 */
char	*append_char_to_result(char *result, char *str, int *i)
{
	char	*temp;

	temp = ft_strjoin_char(result, str[*i]);
	if (!temp)
		return (NULL);
	gc_add_context(TOKENIZER, temp);
	(*i)++;
	return (temp);
}

/**
 * @brief Process string expansion with quote and variable handling
 * @param str String being processed
 * @param env Environment list
 * @param result Initial result string
 * @return Expanded string or NULL on failure
 */
char	*process_expansion_loop(char *str, t_list *env, char *result)
{
	t_expansion_info	info;

	init_expansion_info(&info, result);
	while (str[info.i])
	{
		if (!info.in_quotes && (str[info.i] == '\'' || str[info.i] == '\"'))
			update_quote_type(&info.in_quotes, &info.quote_type, str[info.i],
				&info.i);
		else if (info.in_quotes && str[info.i] == info.quote_type)
			update_quote_type_neg(&info.in_quotes, &info.quote_type, &info.i);
		else if (is_expandable_variable(str, info.i, info.quote_type,
				info.in_quotes))
		{
			info.result = process_var_expansion(str, env, info.result, &info.i);
			if (!info.result)
				return (NULL);
		}
		else
		{
			info.result = append_char_to_result(info.result, str, &info.i);
			if (!info.result)
				return (NULL);
		}
	}
	return (info.result);
}
