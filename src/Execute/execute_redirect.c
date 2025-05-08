/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:47:23 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 11:53:18 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
static int	handle_redirect_in(t_node *node, t_list **env)
{
	t_redirect	*redirect;
	int			fd;
	int			saved_fd;
	int			status;

	redirect = (t_redirect *)node->data;
	fd = open(redirect->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redirect->filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (1);
	}
	saved_fd = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	status = execute_node(node->left, env);
	dup2(saved_fd, STDIN_FILENO);
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

	* @param append Boolean flag indicating whether to append to the file (true)
		or truncate it (false)
 * @return The exit status of the command after redirection,
	or 1 if an error occurs
 */
static int	handle_redirect_out(t_node *node, t_list **env, bool append)
{
	t_redirect	*redirect;
	int			fd;
	int			saved_fd;
	int			status;
	int			flags;

	redirect = (t_redirect *)node->data;
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redirect->filename, flags, 0644);
	if (fd == -1)
		return (print_redirect_error(redirect));
	saved_fd = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	status = execute_node(node->left, env);
	dup2(saved_fd, STDOUT_FILENO);
	close(saved_fd);
	return (status);
}

/**
 * @brief Execute a redirection node in the AST
 *
	* This function processes a redirection node from the Abstract Syntax Tree
	(AST)
 * and executes the appropriate redirection operation based on the node type.
 * It supports input redirection (<), output redirection (>),
	output appending (>>),
 * and heredoc (<<) operations. The function delegates to specialized handlers
 * for each type of redirection.
 *
 * @param node The redirection node to execute, containing the type and data for
 *             the redirection operation

	* @param env A pointer to the environment variables list used for variable
				expansion
 * @return The exit status of the command after redirection, or an error code if
 *         the redirection fails
 */
int	execute_redirect(t_node *node, t_list **env)
{
	if (!node || !node->data || !node->left)
		return (1);
	if (node->type == NODE_REDIRECT_IN)
		return (handle_redirect_in(node, env));
	else if (node->type == NODE_REDIRECT_OUT)
		return (handle_redirect_out(node, env));
	else if (node->type == NODE_APPEND)
		return (handle_redirect_out(node, env));
	else if (node->type == NODE_HERE_DOC)
		return (handle_here_doc(node, env));
	return (1);
}
