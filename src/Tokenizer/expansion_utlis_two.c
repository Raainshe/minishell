/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utlis_two.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:22:17 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/01 11:24:40 by ksinn            ###   ########.fr       */
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
		&& (ft_isalnum(str[i + 1]) || str[i + 1] == '_'
			|| str[i + 1] == '?'));
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
