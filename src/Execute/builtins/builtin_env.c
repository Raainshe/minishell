/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:50:38 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/03 00:10:34 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Built-in env command implementation
 *
 * Displays all environment variables that contain an equals sign (=).
 * Each variable is printed on its own line.
 *
 * @param env Linked list containing environment variables
 * @return 0 on success, 1 if env is NULL
 */
int	builtin_env(t_list *env)
{
	t_list	*current;

	if (!env)
		return (1);
	current = env;
	while (current)
	{
		if (ft_strchr((char *)current->content, '='))
			ft_putendl_fd((char *)current->content, STDOUT_FILENO);
		current = current->next;
	}
	return (0);
}
