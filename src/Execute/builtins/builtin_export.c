/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:50:03 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 11:20:11 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid(char *str)
{
	int		i;
	bool	found_equals;

	i = 0;
	found_equals = false;
	if (!str || !str[0])
		return (false);
	if (str[0] != '_' && !ft_isalpha(str[0]))
		return (false);
	while (str[i])
	{
		if (!found_equals)
		{
			if (str[i] == '=')
				found_equals = true;
			else if (str[i] == '+' && str[i + 1] == '=')
			{
				found_equals = true;
				i++;
			}
			else if (str[i] != '_' && !ft_isalpha(str[i])
				&& !ft_isdigit(str[i]))
				return (false);
		}
		else if (str[i] == '=')
			return (false);
		i++;
	}
	return (true);
}

static int	var_len(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=' && !(arg[i] == '+' && arg[i + 1] == '='))
		i++;
	if (arg[i] == '+')
		return (i);
	return (i);
}

static t_list	*find_duplicate(char *arg, t_list **env)
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

static void	print_export(t_list **env)
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
		{
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, "\"", 1);
			if (content[equals_pos + 1])
				write(STDOUT_FILENO, content + equals_pos + 1, len - equals_pos
					- 1);
			write(STDOUT_FILENO, "\"", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
		current = current->next;
	}
}

static char	*prepare_export_arg(char *orig_arg)
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

int	builtin_export(char **args, t_list **env)
{
	int		i;
	char	*arg;
	t_list	*duplicate;
	bool	is_append;

	i = 1;
	if (!args[1])
		return (print_export(env), 0);
	while (args[i])
	{
		if (is_valid(args[i]))
		{
			is_append = has_append_operator(args[i]);
			duplicate = find_duplicate(args[i], env);
			if (is_append && duplicate)
			{
				arg = create_appended_value(duplicate->content, args[i]);
				duplicate->content = arg;
			}
			else
			{
				arg = prepare_export_arg(args[i]);
				gc_add_context(ENVIRON, arg);
				if (duplicate)
					duplicate->content = arg;
				else
					add_arg(env, arg);
			}
		}
		else
			return (print_error(args[i]));
		i++;
	}
	return (0);
}
