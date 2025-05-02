/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_helper.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:35:20 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/03 00:22:17 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Prints the value part of an environment variable
 * 
 * Formats the variable value for export display, adding quotes around
 * the value part and handling edge cases properly.
 *
 * @param content The environment variable string
 * @param equals_pos The position of the equals sign in the string
 * @param len The total length of the string
 */
static void	print_value(char *content, int equals_pos, int len)
{
	write(STDOUT_FILENO, "=", 1);
	write(STDOUT_FILENO, "\"", 1);
	if (content[equals_pos + 1])
		write(STDOUT_FILENO, content + equals_pos + 1, len - equals_pos - 1);
	write(STDOUT_FILENO, "\"", 1);
}

/**
 * @brief Prints the environment variables in export format
 *
 * Formats and displays all environment variables in the format used
 * by the 'export' command (declare -x NAME="VALUE").
 * 
 * @param env Pointer to the environment variables list
 */
void	print_export(t_list **env)
{
	int		len;
	int		variable_len;
	int		equals_pos;
	t_list	*current;
	char	*content;

	current = *env;
	while (current)
	{
		content = (char *)current->content;
		len = strlen(content);
		variable_len = var_len(content);
		equals_pos = variable_len;
		while (content[equals_pos] && content[equals_pos] != '=')
			equals_pos++;
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		write(STDOUT_FILENO, content, variable_len);
		if (content[equals_pos] == '=')
			print_value(content, equals_pos, len);
		write(STDOUT_FILENO, "\n", 1);
		current = current->next;
	}
}

/**
 * @brief Gets the length of the variable name in an environment string
 * 
 * Calculates the length of the variable name part before any equals sign
 * or append operator (+=).
 *
 * @param arg The environment variable string
 * @return The length of the variable name
 */
int	var_len(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=' && !(arg[i] == '+' && arg[i + 1] == '='))
		i++;
	if (arg[i] == '+')
		return (i);
	return (i);
}

/**
 * @brief Finds a duplicate variable in the environment
 *
 * Searches the environment for a variable with the same name as the
 * argument. The comparison is based on the variable name only, ignoring
 * any value part.
 *
 * @param arg The variable to find
 * @param env Pointer to the environment variables list
 * @return Pointer to the found list node or NULL if not found
 */
t_list	*find_duplicate(char *arg, t_list **env)
{
	t_list	*current;
	int		var_length;

	var_length = var_len(arg);
	current = *env;
	while (current)
	{
		if (ft_strncmp(current->content, arg, var_length) == 0
			&& (((char *)current->content)[var_length] == '='
			|| ((char *)current->content)[var_length] == '\0'))
			return (current);
		current = current->next;
	}
	return (NULL);
}
