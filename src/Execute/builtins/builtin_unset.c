/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:50:20 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/24 15:23:40 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	valid_arg(char *arg)
{
	int	i;

	if (!arg)
		return (FALSE);
	if (arg[0] != '_' && !ft_isalpha(arg[0]))
		return (FALSE);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != '_' && !ft_isalnum(arg[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static void	find_and_remove(char *arg, t_list **env)
{
	t_list	*prev;
	t_list	*current;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->content, arg, ft_strlen(arg)) == 0
			&& ((char *)current->content)[ft_strlen(arg)] == '=')
		{
			if (!prev)
				*env = current->next;
			else
				prev->next = current->next;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(char **args, t_list **env)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!valid_arg(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			i++;
			continue ;
		}
		find_and_remove(args[i], env);
		i++;
	}
	return (0);
}
