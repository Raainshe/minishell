/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:49:00 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/04 13:07:01 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_chdir_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}
int	builtin_cd(char **args)
{
	char	*path;
	char	current_dir[PATH_MAX];
	char	*old_pwd;

	if (!getcwd(current_dir, PATH_MAX))
		return (1);
	if (!args[1])
		path = getenv("HOME");
	else if (ft_strncmp(args[1], "-", 1) == 0)
		path = getenv("OLDPWD");
	else
		path = args[1];
	if (!path)
		return (1);
	if (chdir(path) != 0)
		return (print_chdir_error(path), 1);
	old_pwd = ft_strdup(current_dir);
	if (!old_pwd)
		return (1);
	// TODO: uppdate env
	return (0);
}
