/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:50:03 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/08 14:58:44 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_append_operator(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
		i++;
	return (str[i] == '+' && str[i + 1] == '=');
}

char	*create_appended_value(char *existing, char *new_value)
{
	char	*result;
	char	*existing_value;
	char	*new_arg_value;
	int		existing_var_len;

	existing_var_len = var_len(existing);
	if (((char *)existing)[existing_var_len] == '=')
		existing_value = existing + existing_var_len + 1;
	else
		existing_value = "";
	new_arg_value = new_value + var_len(new_value) + 2;
	result = ft_calloc(strlen(existing_value) + strlen(new_arg_value)
			+ var_len(existing) + 2, sizeof(char));
	gc_add_context(ENVIRON, result);
	ft_strlcpy(result, existing, existing_var_len + 1);
	ft_strlcat(result, "=", strlen(result) + 2);
	ft_strlcat(result, existing_value, strlen(result) + strlen(existing_value)
		+ 1);
	ft_strlcat(result, new_arg_value, strlen(result) + strlen(new_arg_value)
		+ 1);
	return (result);
}

static void	print_export(t_list **env)
{
	t_print_export	pe;

	pe.current = *env;
	while (pe.current)
	{
		pe.content = (char *)pe.current->content;
		pe.len = strlen(pe.content);
		pe.variable_len = var_len(pe.content);
		pe.equals_pos = pe.variable_len;
		while (pe.content[pe.equals_pos] && pe.content[pe.equals_pos] != '=')
			pe.equals_pos++;
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		write(STDOUT_FILENO, pe.content, pe.variable_len);
		if (pe.content[pe.equals_pos] == '=')
		{
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, "\"", 1);
			if (pe.content[pe.equals_pos + 1])
				write(STDOUT_FILENO, pe.content + pe.equals_pos + 1, pe.len
					- pe.equals_pos - 1);
			write(STDOUT_FILENO, "\"", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
		pe.current = pe.current->next;
	}
}

char	*prepare_export_arg(char *orig_arg)
{
	int		var_length;
	char	*new_arg;

	if (!has_append_operator(orig_arg))
		return (ft_strdup(orig_arg));
	var_length = var_len(orig_arg);
	new_arg = ft_calloc(strlen(orig_arg), sizeof(char));
	gc_add_context(ENVIRON, new_arg);
	ft_strlcpy(new_arg, orig_arg, var_length + 1);
	ft_strlcat(new_arg, "=", strlen(new_arg) + 2);
	ft_strlcat(new_arg, orig_arg + var_length + 2, strlen(new_arg)
		+ strlen(orig_arg + var_length + 2) + 1);
	return (new_arg);
}

/**
 * Built-in export command implementation
 * With no arguments, prints all environment variables
 * With arguments, sets or modifies environment variables
 */
int	builtin_export(char **args, t_list **env)
{
	int	i;
	int	result;

	if (!args[1])
		return (print_export(env), 0);
	result = 0;
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], env) != 0)
			result = 1;
		i++;
	}
	return (result);
}
