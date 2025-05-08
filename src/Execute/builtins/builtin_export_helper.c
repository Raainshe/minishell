/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:13:27 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/08 14:39:15 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
