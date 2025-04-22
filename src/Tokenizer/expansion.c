/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:20:38 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/22 14:09:55 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Process a single character during expansion
 * @param str Original string being processed
 * @param env Environment list
 * @param result Current result string
 * @param i Pointer to current position in string
 * @return Updated result string or NULL on failure
 */
static char	*process_expansion_char(char *str, t_list *env, char *result,
		int *i)
{
	char	*temp;

	if (str[*i] == '$' && str[*i + 1] == '?')
		return (process_exit_code(result, i));
	if (str[*i] == '$' && str[*i + 1] && (ft_isalnum(str[*i + 1]) || str[*i
			+ 1] == '_'))
	{
		temp = process_variable(str, env, result, i);
		if (!temp)
			return (NULL);
		return (temp);
	}
	else
	{
		temp = ft_strjoin_char(result, str[*i]);
		if (!temp)
			return (NULL);
		gc_add_context(TOKENIZER, temp);
		(*i)++;
		return (temp);
	}
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

	result = ft_strdup("");
	if (!result)
		return (NULL);
	gc_add_context(TOKENIZER, result);
	i = 0;
	while (str[i])
	{
		result = process_expansion_char(str, env, result, &i);
		if (!result)
			return (NULL);
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
 * @brief Handle expansion of strings inside double quotes
 * @param str String to expand (with double quotes)
 * @param env Environment list
 * @return Expanded string or NULL on failure
 */
static char	*expand_double_quotes(char *str, t_list *env)
{
	char	*trimmed;
	char	*result;

	trimmed = ft_strtrim(str, "\"");
	if (!trimmed)
		return (NULL);
	gc_add_context(TOKENIZER, trimmed);
	result = expand_variables(trimmed, env);
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
		return (gc_add_context(TOKENIZER, trimmed), trimmed);
	}
	else if (str[0] == '\"')
		expanded = expand_double_quotes(str, env);
	else
		expanded = expand_variables(str, env);
	return (expanded);
}
