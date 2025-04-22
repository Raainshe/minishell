/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:04:13 by rmakoni           #+#    #+#             */
/*   Updated: 2025/04/22 12:15:58 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Process a variable expansion at the current position
 * @param str String containing variables
 * @param env Environment list
 * @param result Current result string
 * @param i_ptr Pointer to current index (will be updated)
 * @return Updated result string or NULL on failure
 */
char	*process_variable(char *str, t_list *env, char *result, int *i_ptr)
{
	char	*temp;
	char	*var_name;
	char	*var_value;

	var_name = extract_var_name(str + *i_ptr);
	if (!var_name)
		return (NULL);
	var_value = get_env_value(var_name, env);
	if (!var_value)
		var_value = "";
	temp = ft_strjoin(result, var_value);
	if (!temp)
		return (NULL);
	gc_add_context(TOKENIZER, temp);
	*i_ptr += ft_strlen(var_name) + 1;
	return (temp);
}
