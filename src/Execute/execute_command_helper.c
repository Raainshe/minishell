/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:15:42 by rmakoni           #+#    #+#             */
/*   Updated: 2025/04/27 11:23:19 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_builtin_error(char *str)
{
	ft_putstr_fd("minishell: builtin command not implemented: ", STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
}