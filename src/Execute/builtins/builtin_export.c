/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:50:03 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/07 17:31:54 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid(char *str)
{
	int	i;
	int	equal_signs;

	i = 0;
	equal_signs = 0;
	if (!str || !str[0])
		return (false);
	if (str[0] != '_' && !ft_isalpha(str[0]))
		return (false);
	while (str[i])
	{
		if (str[i] == '=')
			equal_signs++;
		if ((str[i] != '_' && !ft_isalnum(str[i]) && str[i] != '=')
			|| equal_signs > 1)
			return (false);
		i++;
	}
	return (true);
}

int	builtin_export(char **args, t_list *env)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (is_valid(args[i]))
			ft_lstadd_back(&env, ft_lstnew(args[i]));
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
