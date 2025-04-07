/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:46:48 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/07 15:22:39 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if a command is a built-in
 * @param cmd The command name to check
 * @return 1 if it's a built-in, 0 otherwise
 */
static int	is_builtin(char *cmd)
{
	char	*lower_cmd;

	if (!cmd)
		return (0);
	lower_cmd = ft_strlower(cmd);
	if (!lower_cmd)
		return (0);
	if (ft_strncmp(lower_cmd, "echo", 5) == 0 || ft_strncmp(lower_cmd, "cd",
			3) == 0 || ft_strncmp(lower_cmd, "pwd", 4) == 0
		|| ft_strncmp(lower_cmd, "export", 7) == 0 || ft_strncmp(lower_cmd,
			"unset", 6) == 0 || ft_strncmp(lower_cmd, "env", 4) == 0
		|| ft_strncmp(lower_cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

/**
 * @brief Find the full path of a command
 * @param cmd The command to find
 * @param env The environment variables
 * @return The full path to the command or NULL if not found
 */
static char	*find_command_path(char *cmd, t_list *env)
{
	char	*path_var;
	char	**paths;
	char	*full_path;
	int		i;
	char	*dir_with_slash;

	if (!cmd || !env)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	i = 0;
	path_var = NULL;
	while (env)
	{
		if (ft_strncmp((char *)env->content, "PATH=", 5) == 0)
		{
			path_var = (char *)env->content + 5;
			break ;
		}
		env = env->next;
	}
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	gc_add_context(EXECUTOR, paths);
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
 * @brief Execute a built-in command
 * @param args The command arguments
 * @param env The environment variables
 * @return Exit status of the built-in command
 */
static int	execute_builtin(char **args, t_list *env)
{
	char	*lower_cmd;

	if (!args || !args[0])
		return (1);
	// For now, we'll just print a message that the builtin is not implemented
	// Later each builtin will be implemented separately
	(void)env;
	lower_cmd = ft_strlower(args[0]);
	if (!lower_cmd)
		return (0);
	if (ft_strncmp(lower_cmd, "pwd", 4) == 0)
		return (builtin_pwd(args));
	else if (ft_strncmp(lower_cmd, "echo", 5) == 0)
		return (builtin_echo(args));
	else if (ft_strncmp(lower_cmd, "exit", 5) == 0)
		return (builtin_exit(args));
	else if (ft_strncmp(lower_cmd, "cd", 3) == 0)
		return (builtin_cd(args));
	else if (ft_strncmp(lower_cmd, "env", 4) == 0)
		return (builtin_env(env));
	else if (ft_strncmp(lower_cmd, "export", 7) == 0)
		return (builtin_export(args, env));
	else if (ft_strncmp(lower_cmd, "unset", 6) == 0)
		return (builtin_unset(args, env));
	else
	{
		ft_putstr_fd("minishell: builtin command not implemented: ",
			STDERR_FILENO);
		ft_putendl_fd(args[0], STDERR_FILENO);
	}
	return (0);
}

int	execute_command(t_node *node, t_list *env)
{
	t_command	*command;
	pid_t		pid;
	int			status;
	char		*path;
	char		**env_array;

	if (!node || !node->data)
		return (1);
	command = (t_command *)node->data;
	if (!command->args || !command->args[0])
		return (0);
	if (is_builtin(command->args[0]))
		return (execute_builtin(command->args, env));
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		path = find_command_path(command->args[0], env);
		if (!path)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(command->args[0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
			exit(127);
		}
		env_array = convert_env_to_array(env);
		if (!env_array)
		{
			perror("minishell: malloc");
			exit(1);
		}
		execve(path, command->args, env_array);
		perror("minishell: execve");
		exit(126);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
