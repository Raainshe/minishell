/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:50:03 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/08 16:07:05 by ksinn            ###   ########.fr       */
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
			else if (str[i] != '_' && !ft_isalpha(str[i]))
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
	while (arg[i] && arg[i] != '=')
		i++;
	return (++i);
}

static t_list	*find_duplicate(char *arg, t_list **env)
{
	t_list	*current;

	current = *env;
	while (current)
	{
		if (ft_strncmp(current->content, arg, var_len(arg)) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static void	print_export(t_list **env)
{
	int		len;
	int		variable_len;
	t_list	*current;

	current = *env;
	while (current)
	{
		len = strlen(current->content);
		variable_len = var_len(current->content);
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		write(STDOUT_FILENO, current->content, variable_len);
		write(STDOUT_FILENO, "\"", 1);
		write(STDOUT_FILENO, &(current->content[variable_len]), len
			- variable_len);
		write(STDOUT_FILENO, "\"\n", 2);
		current = current->next;
	}
}

int	builtin_export(char **args, t_list **env)
{
	int		i;
	char	*arg;
	t_list	*duplicate;

	i = 1;
	if (!args[1])
		return (print_export(env), 0);
	while (args[i])
	{
		if (is_valid(args[i]))
		{
			arg = ft_strdup(args[i]);
			gc_add_context(ENVIRON, arg);
			duplicate = find_duplicate(args[i], env);
			if (duplicate)
				duplicate->content = arg;
			else
				ft_lstadd_back(env, ft_lstnew(arg));
		}
		else
		{
			ft_putstr_fd("export: ", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
			return (1);
		}
		i++;
	}
	return (0);
}
