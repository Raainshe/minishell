/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:50:03 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 12:37:23 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_append_operator(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
		i++;
	return (str[i] == '+' && str[i + 1] == '=');
}

static char	*create_appended_value(char *existing, char *new_value)
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

static char	*prepare_export_arg(char *orig_arg)
{
	int		var_length;
	char	*new_arg;

	if (!has_append_operator(orig_arg))
		return (ft_strdup(orig_arg));
	var_length = var_len(orig_arg);
	new_arg = ft_calloc(strlen(orig_arg), sizeof(char));
	ft_strlcpy(new_arg, orig_arg, var_length + 1);
	ft_strlcat(new_arg, "=", strlen(new_arg) + 2);
	ft_strlcat(new_arg, orig_arg + var_length + 2, strlen(new_arg)
		+ strlen(orig_arg + var_length + 2) + 1);
	return (new_arg);
}

/**
 * @brief Process a single export argument
 *
 * @param arg The argument to process
 * @param env The environment variables list
 * @return int 0 on success, non-zero on error
 */
static int	process_export_arg(char *arg, t_list **env)
{
	char	*new_arg;
	t_list	*duplicate;
	bool	is_append;

	if (!is_valid(arg))
		return (print_error(arg));
	is_append = has_append_operator(arg);
	duplicate = find_duplicate(arg, env);
	if (is_append && duplicate)
	{
		new_arg = create_appended_value(duplicate->content, arg);
		duplicate->content = new_arg;
	}
	else
	{
		new_arg = prepare_export_arg(arg);
		gc_add_context(ENVIRON, new_arg);
		if (duplicate)
			duplicate->content = new_arg;
		else
			add_arg(env, new_arg);
	}
	return (0);
}

/**
 * @brief Export variables to the environment
 *
 * @param args Command arguments array
 * @param env The environment variables list
 * @return int Exit status (0 on success, non-zero on error)
 */
int	builtin_export(char **args, t_list **env)
{
	int	i;
	int	status;

	i = 1;
	if (!args[1])
		return (print_export(env), 0);
	while (args[i])
	{
		status = process_export_arg(args[i], env);
		if (status != 0)
			return (status);
		i++;
	}
	return (0);
}
