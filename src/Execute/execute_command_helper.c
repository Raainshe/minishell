/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:15:42 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/01 11:39:43 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_builtin_error(char *str)
{
	ft_putstr_fd("minishell: builtin command not implemented: ", STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
}

/**
 * @brief Get the PATH environment variable value
 * @param env The environment variables list
 * @return The PATH value or NULL if not found
 */
static char	*get_path_var(t_list *env)
{
	char	*path_var;

	path_var = NULL;
	while (env)
	{
		if (ft_strncmp("PATH=", (char *)env->content, 5) == 0)
		{
			path_var = (char *)env->content + 5;
			break ;
		}
		env = env->next;
	}
	return (path_var);
}

/**
 * @brief Check if a command is a direct path
 * @param cmd The command to check
 * @return The duplicated command if it's a path, NULL otherwise
 */
static char	*check_direct_path(char *cmd)
{
	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

/**
 * @brief Try to find command in the specified directory
 * @param dir Directory to search in
 * @param cmd Command name to find
 * @return Full path if found, NULL otherwise
 */
static char	*try_command_in_dir(char *dir, char *cmd)
{
	char	*dir_with_slash;
	char	*full_path;

	dir_with_slash = ft_strjoin(dir, "/");
	if (!dir_with_slash)
		return (NULL);
	gc_add_context(EXECUTOR, dir_with_slash);
	full_path = ft_strjoin(dir_with_slash, cmd);
	if (!full_path)
		return (NULL);
	gc_add_context(EXECUTOR, full_path);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	return (NULL);
}

/**
 * @brief Find the full path of a command
 * @param cmd The command to find
 * @param env The environment variables
 * @return The full path to the command or NULL if not found
 */
char	*find_command_path(char *cmd, t_list *env)
{
	t_find_command_path	fcp;

	if (!cmd || !env)
		return (NULL);
	fcp.direct_path = check_direct_path(cmd);
	if (fcp.direct_path)
		return (fcp.direct_path);
	fcp.path_var = get_path_var(env);
	if (!fcp.path_var)
		return (NULL);
	fcp.paths = ft_split(fcp.path_var, ':');
	if (!fcp.paths)
		return (NULL);
	gc_add_context(EXECUTOR, fcp.paths);
	fcp.i = 0;
	while (fcp.paths[fcp.i])
	{
		fcp.full_path = try_command_in_dir(fcp.paths[fcp.i], cmd);
		if (fcp.full_path)
			return (fcp.full_path);
		fcp.i++;
	}
	return (NULL);
}
