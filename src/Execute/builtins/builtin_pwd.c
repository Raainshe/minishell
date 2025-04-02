/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:49:43 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/02 13:33:17 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: Error if options
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
