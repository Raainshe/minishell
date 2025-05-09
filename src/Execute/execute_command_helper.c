/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:15:42 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/09 13:07:19 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_builtin_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
}

/**
 * @brief Get the PATH environment variable
 * @param env The environment variables list
 * @return The PATH string or NULL if not found
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
 * @brief Search for a command in the PATH directories
 * @param cmd The command to find
 * @param paths Array of directories to search in
 * @return The full path to the command or NULL if not found
 */
static char	*search_in_paths(char *cmd, char **paths)
{
	char	*full_path;
	char	*dir_with_slash;
	int		i;

	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		dir_with_slash = ft_strjoin(paths[i], "/");
		if (!dir_with_slash)
			return (NULL);
		gc_add_context(EXECUTOR, dir_with_slash);
		full_path = ft_strjoin(dir_with_slash, cmd);
		if (!full_path)
			return (NULL);
		gc_add_context(EXECUTOR, full_path);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
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
	char	*path_var;
	char	**paths;
	char	*full_path;

	if (!cmd || !env || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_path_var(env);
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	gc_add_context(EXECUTOR, paths);
	full_path = search_in_paths(cmd, paths);
	return (full_path);
}
