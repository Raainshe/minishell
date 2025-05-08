/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_out.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:10:38 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/08 16:25:28 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Determines file open flags based on the redirection type
 *

* @param node_type The type of redirection node
 * (NODE_REDIRECT_OUT or NODE_APPEND)
 * @return The appropriate file open flags
 */
static int	get_redirection_flags(int node_type)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (node_type == NODE_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	return (flags);
}

/**
 * @brief Builds a stack of redirection nodes
 *
 * @param node The starting redirection node
 * @param redirect_stack Array to store the redirection nodes
 * @return The command node after all redirections have been processed
 */
static t_node	*build_redirect_stack(t_node *node, t_node *redirect_stack[100],
		int *stack_size)
{
	t_node	*cmd_node;

	*stack_size = 0;
	cmd_node = node;
	while (cmd_node && (cmd_node->type == NODE_REDIRECT_OUT
			|| cmd_node->type == NODE_APPEND))
	{
		redirect_stack[(*stack_size)++] = cmd_node;
		cmd_node = cmd_node->left;
	}
	return (cmd_node);
}

/**
 * @brief Processes all intermediate redirection nodes (all but the last)
 *
 * Opens and immediately closes each file to ensure they exist
 * and are writable.
 *
 * @param redirect_stack Array of redirection nodes
 * @param stack_size Pointer to the stack size
 * @return 1 if an error occurs, 0 otherwise
 */
static int	process_intermediate_redirections(t_node *redirect_stack[100],
		int *stack_size)
{
	t_redirect	*redirect;
	int			fd;
	int			flags;

	while (*stack_size > 1)
	{
		(*stack_size)--;
		redirect = (t_redirect *)redirect_stack[*stack_size]->data;
		flags = get_redirection_flags(redirect_stack[*stack_size]->type);
		fd = open(redirect->filename, flags, 0644);
		if (fd == -1)
			return (print_redirect_error(redirect));
		close(fd);
	}
	return (0);
}

/**
 * @brief Processes the final redirection and executes the command
 *
 * Opens the file for the last redirection, redirects STDOUT to it,
 * executes the command, and then restores STDOUT.
 *
 * @param redirect_stack Array of redirection nodes
 * @param stack_size Pointer to the stack size
 * @param cmd_node The command node to execute
 * @param env A pointer to the environment variables list
 * @return The exit status of the command
 */
static int	process_final_redirection(t_node *redirect_stack[100],
		int *stack_size, t_node *cmd_node, t_list **env)
{
	t_redirect	*redirect;
	int			fd;
	int			saved_fd;
	int			status;
	int			flags;

	(*stack_size)--;
	redirect = (t_redirect *)redirect_stack[*stack_size]->data;
	flags = get_redirection_flags(redirect_stack[*stack_size]->type);
	fd = open(redirect->filename, flags, 0644);
	if (fd == -1)
		return (print_redirect_error(redirect));
	saved_fd = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	status = execute_node(cmd_node, env);
	dup2(saved_fd, STDOUT_FILENO);
	close(saved_fd);
	return (status);
}

/**
 * @brief Handles output redirection to a file
 *
 * This function implements output redirection (> and >>),
	creating or opening the
	* specified file for writing. It supports both truncating the file (>) and
	appending
	* to it (>>). The function saves the original STDOUT to restore it after
	executing
 * the command. If the file cannot be opened or created,
	an error message is displayed.
 *
 * @param node The redirection node containing the filename to write to
 * @param env A pointer to the environment variables list
 * @return The exit status of the command after redirection,
	or 1 if an error occurs
 */
int	handle_redirect_out(t_node *node, t_list **env)
{
	t_node	*cmd_node;
	int		status;
	t_node	*redirect_stack[100];
	int		stack_size;

	cmd_node = build_redirect_stack(node, redirect_stack, &stack_size);
	status = process_intermediate_redirections(redirect_stack, &stack_size);
	if (status != 0)
		return (status);
	if (stack_size > 0)
		return (process_final_redirection(redirect_stack, &stack_size, cmd_node,
				env));
	return (execute_node(cmd_node, env));
}
