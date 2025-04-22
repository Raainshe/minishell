/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:04:13 by rmakoni           #+#    #+#             */
/*   Updated: 2025/04/22 12:26:47 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get the value of an environment variable from the environment list
 * @param var_name Name of the variable to get
 * @param env Environment list
 * @return Value of the variable or NULL if not found
 */
char	*get_env_value(char *var_name, t_list *env)
{
	int		name_len;
	t_list	*current;
	char	*env_entry;

	if (!var_name || !env)
		return (NULL);
	name_len = ft_strlen(var_name);
	current = env;
	while (current)
	{
		env_entry = (char *)current->content;
		if (ft_strncmp(env_entry, var_name, name_len) == 0
			&& env_entry[name_len] == '=')
			return (env_entry + name_len + 1);
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Extracts the variable name from a string
 * @param str String starting with $
 * @return Allocated string with variable name
 */
char	*extract_var_name(char *str)
{
	int		i;
	char	*var_name;

	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	var_name = ft_strndup(str + 1, i - 1);
	if (!var_name)
		return (NULL);
	gc_add_context(TOKENIZER, var_name);
	return (var_name);
}

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
