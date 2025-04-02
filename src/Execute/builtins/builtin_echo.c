/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:43:31 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/02 14:39:20 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Helper function to print arguments for echo
 * @param args Array of argument strings
 * @param start_idx Starting index of arguments to print
 * @return 0 on success, 1 on error
 */
static int	print_args(char **args, int start_idx)
{
	int		i;
	char	*trimmed;

	i = start_idx;
	while (args[i])
	{
		trimmed = ft_strtrim(args[i], "\"\'");
		if (!trimmed)
		{
			gc_free_context(EXECUTOR);
			return (1);
		}
		if (ft_putstr_fd(trimmed, STDOUT_FILENO) == -1)
			return (1);
		if (args[i + 1])
		{
			if (ft_putchar_fd(' ', STDOUT_FILENO) == -1)
				return (1);
		}
		i++;
	}
	return (0);
}

// TODO: \n and I guess other \ do not work as in bash,
// TODO: For now just trimming " and ', need to check if correct (expansion)
/**
 * @brief Print argument strings to standard output
 * @param args Command arguments (args[0] is "echo")
 * @return 0 on success, 1 on error
 */
int	builtin_echo(char **args)
{
	bool	newline;
	int		i;
	int		status;

	if (!args || !args[0])
		return (1);
	newline = true;
	i = 1;
	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		newline = false;
		i++;
	}
	status = print_args(args, i);
	if (status != 0)
		return (status);
	if (newline)
	{
		if (ft_putchar_fd('\n', STDOUT_FILENO) == -1)
			return (1);
	}
	return (0);
}
