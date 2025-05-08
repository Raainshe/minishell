/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:09:22 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/08 16:17:04 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Builds a stack of redirection nodes
 *

	* This function traverses the node tree and collects all input redirection
	* nodes into a stack. It returns the actual command node to execute.
 *
 * @param node The starting node for traversal
 * @param redirect_stack Array to store the redirection nodes
 * @param stack_size Pointer to store the size of the stack
 * @return The command node to execute after redirection
 */
static t_node	*build_redirect_stack(t_node *node, t_node *redirect_stack[100],
		int *stack_size)
{
	t_node	*cmd_node;

	*stack_size = 0;
	cmd_node = node;
	while (cmd_node && cmd_node->type == NODE_REDIRECT_IN)
	{
		redirect_stack[(*stack_size)++] = cmd_node;
		cmd_node = cmd_node->left;
	}
	return (cmd_node);
}

/**
 * @brief Opens the redirection file for reading
 *
 * This function attempts to open the specified file for reading.
 * If the file cannot be opened, an error message is displayed.
 *
 * @param filename The name of the file to open
 * @return The file descriptor on success, -1 on failure
 */
static int	open_redirect_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	}
	return (fd);
}

/**
 * @brief Executes a command with input redirection
 *
 * This function saves the original STDIN, redirects input from the file,
 * executes the command, and restores the original STDIN.
 *
 * @param cmd_node The command node to execute
 * @param fd The file descriptor to read from
 * @param env A pointer to the environment variables list
 * @return The exit status of the command
 */
static int	execute_with_redirection(t_node *cmd_node, int fd, t_list **env)
{
	int	saved_fd;
	int	status;

	saved_fd = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	status = execute_node(cmd_node, env);
	dup2(saved_fd, STDIN_FILENO);
	close(saved_fd);
	return (status);
}

/**
 * @brief Handles input redirection from a file
 *
 * This function implements the input redirection operation (<),
	opening the specified
	* file for reading and redirecting STDIN to read from this file. It saves
	the original
	* STDIN file descriptor to restore it after executing the command. If the
	file cannot
 * be opened, an appropriate error message is displayed.
 *
 * @param node The redirection node containing the filename to read from
 * @param env A pointer to the environment variables list
 * @return The exit status of the command after redirection,
	or 1 if the file cannot be opened
 */
int	handle_redirect_in(t_node *node, t_list **env)
{
	t_redirect	*redirect;
	t_node		*cmd_node;
	int			fd;
	t_node		*redirect_stack[100];
	int			stack_size;

	cmd_node = build_redirect_stack(node, redirect_stack, &stack_size);
	if (stack_size > 0)
	{
		redirect = (t_redirect *)redirect_stack[0]->data;
		fd = open_redirect_file(redirect->filename);
		if (fd == -1)
			return (1);
		return (execute_with_redirection(cmd_node, fd, env));
	}
	return (execute_node(cmd_node, env));
}
