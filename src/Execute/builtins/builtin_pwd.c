/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:49:43 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 15:14:21 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **args)
{
	char	current_dir[PATH_MAX];

	(void)args;
	if (getcwd(current_dir, PATH_MAX) != NULL)
		return (ft_putendl_fd(current_dir, STDOUT_FILENO), 0);
	else
	{
		ft_putstr_fd("pwd: error retrieving current directory: ",
			STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (1);
	}
}
