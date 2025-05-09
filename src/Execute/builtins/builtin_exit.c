/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:51:04 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/09 17:00:57 by rmakoni          ###   ########.fr       */
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

static void	print_exit_msg(int *exit_code)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
	free_gc();
	exit(*exit_code);
}

static void	print_exit_msg_two(int *exit_code, char *str)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	*exit_code = 255;
	free_gc();
	exit(*exit_code);
}

int	builtin_exit(char **args)
{
	int	*exit_code;
	int	numeric_arg_value;

	exit_code = ft_exit_code_holder();
	if (args[1] == NULL)
		print_exit_msg(exit_code);
	if (!is_numeric_arg(args[1]))
		return (print_exit_msg_two(exit_code, args[1]), 255);
	if (args[2] != NULL)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		*exit_code = 1;
		return (1);
	}
	ft_putendl_fd("exit", STDOUT_FILENO);
	numeric_arg_value = ft_atoi(args[1]);
	*exit_code = (unsigned char)numeric_arg_value;
	free_gc();
	exit(*exit_code);
	return (0);
}
