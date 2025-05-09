/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:43:31 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/09 19:50:09 by rmakoni          ###   ########.fr       */
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
	int	i;

	i = start_idx;
	while (args[i])
	{
		if (ft_putstr_fd(args[i], STDOUT_FILENO) == -1)
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

/**
 * @brief Checks if the argument is a valid -n option (e.g., "-n", "-nnn")
 * @param arg The argument string to check
 * @return True if it's a valid -n option, false otherwise
 */
static bool	is_n_option(char *arg)
{
	int	j;

	if (arg[0] == '-' && arg[1] == 'n')
	{
		j = 1;
		while (arg[j] != '\0')
		{
			if (arg[j] != 'n')
				return (false);
			j++;
		}
		return (j > 1);
	}
	return (false);
}

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
	while (args[i] && is_n_option(args[i]))
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
