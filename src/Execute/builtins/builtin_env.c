/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:50:38 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 11:05:22 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
