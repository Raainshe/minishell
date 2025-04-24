/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:13:27 by rmakoni           #+#    #+#             */
/*   Updated: 2025/04/24 14:30:46 by rmakoni          ###   ########.fr       */
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
