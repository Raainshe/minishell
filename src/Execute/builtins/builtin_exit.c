/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:51:04 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/03 00:10:34 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if a string represents a valid number
 * @param str String to check
 * @return 1 if valid number, 0 otherwise
 */
static int	is_numeric_arg(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Built-in exit command implementation
 *
 * Terminates the shell with the specified exit code.
 * If a numeric argument is provided, it's used as the exit code.
 * If the argument is not numeric, exits with status 255.
 * If too many arguments are provided, reports an error but doesn't exit.
 *
 * @param args Command arguments (args[0] is "exit")
 * @return Never returns if successful; returns exit code on error
 */
int	builtin_exit(char **args)
{
	int	*exit_code;

	ft_putendl_fd("exit", STDOUT_FILENO);
	exit_code = ft_exit_code_holder();
	if (args[1])
	{
		if (!is_numeric_arg(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			free_gc();
			exit(255);
		}
		if (args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			*exit_code = 1;
		}
		else
			*exit_code = ft_atoi(args[1]) % 256;
	}
	free_gc();
	exit(*exit_code);
}
