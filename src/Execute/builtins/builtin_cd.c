/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:49:00 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/27 10:36:40 by rmakoni          ###   ########.fr       */
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

static int	update_env(char *pwd, char *old_pwd, t_list **env)
{
	char	*export_pwd;
	char	*export_old_pwd;
	char	*args[4];

	export_pwd = ft_strjoin("PWD=", pwd);
	if (!export_pwd)
		return (1);
	export_old_pwd = ft_strjoin("OLDPWD=", old_pwd);
	if (!export_old_pwd)
	{
		free(export_pwd);
		return (1);
	}
	args[0] = "export";
	args[1] = export_pwd;
	args[2] = export_old_pwd;
	args[3] = NULL;
	builtin_export(args, env);
	free(export_pwd);
	free(export_old_pwd);
	return (0);
}

/**
 * @brief Determines the target path for cd based on arguments
 *
 * @param args Command arguments
 * @param env Environment variables list
 * @return Target path or NULL if an error occurred
 */
static char	*get_cd_path(char **args, t_list **env)
{
	char	*path;

	if (!args[1])
		path = ft_getenv("HOME", *env);
	else if (args[1][0] == '~')
	{
		path = ft_getenv("HOME", *env);
		path = ft_strjoin(path, args[1] + 1);
		gc_add_context(ENVIRON, path);
	}
	else if (ft_strncmp(args[1], "-", 1) == 0)
	{
		path = ft_getenv("OLDPWD", *env);
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
			return (NULL);
		}
	}
	else
		path = args[1];
	return (path);
}

/**
 * @brief Built-in cd command implementation
 *
 * Changes the current working directory and updates PWD and OLDPWD
 * environment variables.
 *
 * @param args Command arguments
 * @param env Environment variables list
 * @return 0 on success, 1 on failure
 */
int	builtin_cd(char **args, t_list **env)
{
	char	*path;
	char	current_dir[PATH_MAX];
	char	*old_pwd;

	if (!getcwd(current_dir, PATH_MAX))
		return (1);
	path = get_cd_path(args, env);
	if (!path)
		return (1);
	if (chdir(path) != 0)
		return (print_chdir_error(path), 1);
	old_pwd = ft_strdup(current_dir);
	if (!old_pwd)
		return (1);
	gc_add_context(ENVIRON, old_pwd);
	if (!getcwd(current_dir, PATH_MAX))
		return (1);
	return (update_env(current_dir, old_pwd, env));
}
