/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:13:27 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/01 12:32:32 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**

 * @brief Check if a character is valid in a variable name or handle
 *        special chars
 * @param c The character to check
 * @param next_c The next character in the string
 * @param found_equals Pointer to found_equals flag to update
 * @param i_ptr Pointer to index to potentially increment
 * @return true if valid, false if invalid
 */
static bool	check_char_validity(char c, char next_c, bool *found_equals,
		int *i_ptr)
{
	if (c == '=')
	{
		*found_equals = true;
		return (true);
	}
	else if (c == '+' && next_c == '=')
	{
		*found_equals = true;
		(*i_ptr)++;
		return (true);
	}
	else if (c == '_' || ft_isalpha(c) || ft_isdigit(c))
		return (true);
	return (false);
}

/**
 * @brief Check if the variable identifier is valid for export
 * @param str The variable to check
 * @return true if valid, false otherwise
 */
bool	is_valid(char *str)
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
			if (!check_char_validity(str[i], str[i + 1], &found_equals, &i))
				return (false);
		}
		else if (str[i] == '=')
			return (false);
		i++;
	}
	return (true);
}

int	print_error(char *arg)
{
	ft_putstr_fd("export: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	return (1);
}

void	add_arg(t_list **env, char *arg)
{
	t_list	*tmp;

	tmp = ft_lstnew(arg);
	gc_add_context(ENVIRON, tmp);
	ft_lstadd_back(env, tmp);
}

/**
 * Updates an existing environment variable with an appended value
 */
static void	update_with_append(t_list *duplicate, char *arg)
{
	char	*appended_value;

	appended_value = create_appended_value(duplicate->content, arg);
	duplicate->content = appended_value;
}

/**
 * Updates or adds a new environment variable
 */
static void	update_or_add_var(t_list **env, char *arg, t_list *duplicate)
{
	char	*prepared_arg;

	prepared_arg = prepare_export_arg(arg);
	gc_add_context(ENVIRON, prepared_arg);
	if (duplicate)
		duplicate->content = prepared_arg;
	else
		add_arg(env, prepared_arg);
}

/**
 * Processes a single export argument
 * Returns 0 on success, 1 on error
 */
int	process_export_arg(char *arg, t_list **env)
{
	t_list	*duplicate;
	bool	is_append;

	if (!is_valid(arg))
		return (print_error(arg));
	is_append = has_append_operator(arg);
	duplicate = find_duplicate(arg, env);
	if (is_append && duplicate)
		update_with_append(duplicate, arg);
	else
		update_or_add_var(env, arg, duplicate);
	return (0);
}
