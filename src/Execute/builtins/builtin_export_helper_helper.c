/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper_helper.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:35:20 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 12:37:33 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_value(char *content, int equals_pos, int len)
{
	write(STDOUT_FILENO, "=", 1);
	write(STDOUT_FILENO, "\"", 1);
	if (content[equals_pos + 1])
		write(STDOUT_FILENO, content + equals_pos + 1, len - equals_pos - 1);
	write(STDOUT_FILENO, "\"", 1);
}

void	print_export(t_list **env)
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
			print_value(content, equals_pos, len);
		write(STDOUT_FILENO, "\n", 1);
		current = current->next;
	}
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
