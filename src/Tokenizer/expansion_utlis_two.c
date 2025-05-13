/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utlis_two.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:22:17 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/13 13:22:15 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the current position contains an expandable variable
 * @param str String being processed
 * @param i Current position in string
 * @param quote_type Current quote type (' or ")
 * @param in_quotes Flag indicating if we're inside quotes
 * @return true if expandable, false otherwise
 */
bool	is_expandable_variable(char *str, int i, char quote_type, int in_quotes)
{
	return ((quote_type == '\"' || !in_quotes) && str[i] == '$' && str[i + 1]
		&& (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'));
}

/**
 * @brief Initialize expansion info struct
 * @param info Pointer to expansion info struct
 * @param result Initial result string
 */
void	init_expansion_info(t_expansion_info *info, char *result)
{
	info->i = 0;
	info->in_quotes = 0;
	info->quote_type = '\0';
	info->result = result;
}

/**
 * @brief Handle initial tilde expansion at the beginning of a string
 * @param str String being processed
 * @param env Environment list
 * @param info Expansion info structure
 * @return Updated result string or NULL on failure
 */
char	*handle_initial_tilde(char *str, t_list *env, t_expansion_info *info)
{
	if (str[info->i] == '~' && (!str[info->i + 1] || str[info->i + 1] == '/'
			|| ft_isspace(str[info->i + 1])))
	{
		info->result = process_tilde_expansion(str, env, info->result,
				&info->i);
		if (!info->result)
			return (NULL);
	}
	return (info->result);
}

/**
 * @brief Handle quote opening and closing during expansion
 * @param str String being processed
 * @param info Expansion info structure
 * @return Updated expansion info result or NULL on failure
 */
char	*handle_quotes(char *str, t_expansion_info *info)
{
	if (!info->in_quotes && (str[info->i] == '\'' || str[info->i] == '\"'))
		update_quote_type(&info->in_quotes, &info->quote_type, str[info->i],
			&info->i);
	else if (info->in_quotes && str[info->i] == info->quote_type)
		update_quote_type_neg(&info->in_quotes, &info->quote_type, &info->i);
	return (info->result);
}

/**
 * @brief Process a single character during expansion
 * @param str String being processed
 * @param env Environment list
 * @param info Expansion info structure
 * @return Updated result string or NULL on failure
 */
char	*process_expansion_char(char *str, t_list *env, t_expansion_info *info)
{
	if ((!info->in_quotes && (str[info->i] == '\'' || str[info->i] == '\"'))
		|| (info->in_quotes && str[info->i] == info->quote_type))
	{
		return (handle_quotes(str, info));
	}
	else if (is_expandable_variable(str, info->i, info->quote_type,
			info->in_quotes))
	{
		info->result = process_var_expansion(str, env, info->result, &info->i);
		if (!info->result)
			return (NULL);
	}
	else
	{
		info->result = append_char_to_result(info->result, str, &info->i);
		if (!info->result)
			return (NULL);
	}
	return (info->result);
}
