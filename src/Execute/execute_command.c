/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:46:48 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 11:39:37 by ksinn            ###   ########.fr       */
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
	if (ft_strncmp("echo", lower_cmd, 5) == 0 || ft_strncmp("cd", lower_cmd,
			3) == 0 || ft_strncmp("pwd", lower_cmd, 4) == 0
		|| ft_strncmp("export", lower_cmd, 7) == 0 || ft_strncmp("unset",
			lower_cmd, 6) == 0 || ft_strncmp("env", lower_cmd, 4) == 0
		|| ft_strncmp("exit", lower_cmd, 5) == 0)
		return (1);
	return (0);
}

/**
 * @brief Execute a built-in command
 * @param args The command arguments
 * @param env The environment variables
 * @return Exit status of the built-in command
 */
static int	execute_builtin(char **args, t_list **env)
{
	char	*lower_cmd;

	if (!args || !args[0])
		return (1);
	(void)env;
	lower_cmd = ft_strlower(args[0]);
	if (!lower_cmd)
		return (0);
	if (ft_strncmp("pwd", lower_cmd, 4) == 0)
		return (builtin_pwd(args));
	else if (ft_strncmp("echo", lower_cmd, 5) == 0)
		return (builtin_echo(args));
	else if (ft_strncmp("exit", lower_cmd, 5) == 0)
		return (builtin_exit(args));
	else if (ft_strncmp("cd", lower_cmd, 3) == 0)
		return (builtin_cd(args, env));
	else if (ft_strncmp("env", lower_cmd, 4) == 0)
		return (builtin_env(*env));
	else if (ft_strncmp("export", lower_cmd, 7) == 0)
		return (builtin_export(args, env));
	else if (ft_strncmp("unset", lower_cmd, 6) == 0)
		return (builtin_unset(args, env));
	else
		print_builtin_error(args[0]);
	return (0);
}

/**
 * @brief Function to handle the child process execution
 * @param command The command to execute
 * @param env The environment variables
 */
static void	execute_child_process(t_command *command, t_list *env)
{
	char	*path;
	char	**env_array;

	reset_signals();
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

/**
 * @brief Handle the parent process after forking
 * @param pid The child process ID
 * @return Exit status of the child process
 */
static int	handle_parent_process(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			g_signal_received = SIGINT;
		else if (WTERMSIG(status) == SIGQUIT)
			g_signal_received = SIGQUIT;
		return (128 + WTERMSIG(status));
	}
	return (1);
}

/**
 * @brief Execute a command, either builtin or external
 * @param node The command node
 * @param env The environment variables
 * @return Exit status of the command
 */
int	execute_command(t_node *node, t_list **env)
{
	t_command	*command;
	pid_t		pid;

	if (!node || !node->data)
		return (1);
	command = (t_command *)node->data;
	if (!command->args || !command->args[0])
		return (0);
	if (is_builtin(command->args[0]))
		return (execute_builtin(command->args, env));
	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
		execute_child_process(command, *env);
	return (handle_parent_process(pid));
}
