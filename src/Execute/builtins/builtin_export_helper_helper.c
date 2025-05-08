/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_helper.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:54:19 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/08 14:58:59 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_char_validity(char c, char next_c, bool *found_equals,
		int *i_ptr)
{
	if (!*found_equals)
	{
		if (c == '=')
			*found_equals = true;
		else if (c == '+' && next_c == '=')
		{
			*found_equals = true;
			(*i_ptr)++;
		}
		else if (c != '_' && !ft_isalpha(c) && !ft_isdigit(c))
			return (false);
	}
	else if (c == '=')
		return (false);
	return (true);
}

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
		if (!check_char_validity(str[i], str[i + 1], &found_equals, &i))
			return (false);
		i++;
	}
	return (true);
}

int	var_len(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=' && !(arg[i] == '+' && arg[i + 1] == '='))
		i++;
	if (arg[i] == '+')
		return (i);
	return (i);
}

t_list	*find_duplicate(char *arg, t_list **env)
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
