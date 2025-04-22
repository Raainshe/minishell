/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:20:38 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/22 12:11:48 by rmakoni          ###   ########.fr       */
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
 * @brief Expand a string with environment variables
 * @param str String to expand
 * @param env Environment list
 * @return Expanded string or NULL on failure
 */
char	*expand_variables(char *str, t_list *env)
{
	int		i;
	char	*result;
	char	*temp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	gc_add_context(TOKENIZER, result);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i
				+ 1] == '_'))
		{
			temp = process_variable(str, env, result, &i);
			if (!temp)
				return (NULL);
			result = temp;
		}
		else
		{
			temp = ft_strjoin_char(result, str[i]);
			if (!temp)
				return (NULL);
			gc_add_context(TOKENIZER, temp);
			result = temp;
			i++;
		}
	}
	return (result);
}

/**
 * @brief Helper function to join a string and a character
 * @param str String to join with
 * @param c Character to append
 * @return New string or NULL on failure
 */
char	*ft_strjoin_char(char *str, char c)
{
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	result = (char *)ft_calloc(ft_strlen(str) + 2, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}

/**
 * @brief Expand a token string, handling quotes and environment variables
 * @param str String to expand
 * @param env Environment list
 * @return Expanded string or NULL on failure
 */
char	*expand(char *str, t_list *env)
{
	char	*trimmed;
	char	*expanded;

	if (!str || !str[0])
		return (NULL);
	if (str[0] == '\'')
	{
		trimmed = ft_strtrim(str, "'");
		if (!trimmed)
			return (NULL);
		gc_add_context(TOKENIZER, trimmed);
		return (trimmed);
	}
	else
	{
		if (str[0] == '\"')
		{
			trimmed = ft_strtrim(str, "\"");
			if (!trimmed)
				return (NULL);
			gc_add_context(TOKENIZER, trimmed);
			expanded = expand_variables(trimmed, env);
		}
		else
			expanded = expand_variables(str, env);
		if (!expanded)
			return (NULL);
		return (expanded);
	}
}
