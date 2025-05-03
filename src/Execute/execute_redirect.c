/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:47:23 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/03 14:13:16 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the heredoc input, reading until the delimiter is encountered
 * @param delimiter The delimiter string that ends the heredoc
 * @param env The environment variables list for expansion
 * @param expand_vars Whether to expand variables in the heredoc content
 * @return The file descriptor to read from, or -1 on error
 */
static int	handle_heredoc(char *delimiter, t_list *env, bool expand_vars)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
		handle_heredoc_child(pipe_fd, delimiter, env, expand_vars);
	else
		return (handle_heredoc_parent(pipe_fd, pid));
	return (-1);
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
static int	handle_redirect_in(t_node *node, t_list **env)
{
	t_redirect	*redirect;
	t_node		*cmd_node;
	int			fd;
	int			saved_fd;
	int			status;
	t_node		*redirect_stack[100];
	int			stack_size;

	stack_size = 0;
	cmd_node = node;
	while (cmd_node && cmd_node->type == NODE_REDIRECT_IN)
	{
		redirect_stack[stack_size++] = cmd_node;
		cmd_node = cmd_node->left;
	}
	if (stack_size > 0)
	{
		// Use the first node in stack (deepest in AST, last in command line)
		redirect = (t_redirect *)redirect_stack[0]->data;
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
		status = execute_node(cmd_node, env);
		dup2(saved_fd, STDIN_FILENO);
		close(saved_fd);
		return (status);
	}
	return (execute_node(cmd_node, env));
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
static int	handle_redirect_out(t_node *node, t_list **env)
{
	t_redirect	*redirect;
	t_node		*cmd_node;
	int			fd;
	int			saved_fd;
	int			status;
	int			flags;
	t_node		*redirect_stack[100];
	int			stack_size;

	// Push all redirections onto a stack to reverse their order
	stack_size = 0;
	cmd_node = node;
	while (cmd_node && (cmd_node->type == NODE_REDIRECT_OUT
			|| cmd_node->type == NODE_APPEND))
	{
		redirect_stack[stack_size++] = cmd_node;
		cmd_node = cmd_node->left;
	}
	// Process files in reverse order (last specified redirection first)
	// Create empty files for all redirections except the last one
	while (stack_size > 1)
	{
		stack_size--;
		redirect = (t_redirect *)redirect_stack[stack_size]->data;
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(redirect->filename, flags, 0644);
		if (fd == -1)
			return (print_redirect_error(redirect));
		close(fd);
	}
	// For the last file (the first in command syntax), redirect STDOUT
	if (stack_size > 0)
	{
		stack_size--;
		redirect = (t_redirect *)redirect_stack[stack_size]->data;
		flags = O_WRONLY | O_CREAT;
		if (redirect_stack[stack_size]->type == NODE_APPEND)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
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
	return (execute_node(cmd_node, env));
}

/**
 * @brief Handles heredoc input redirection
 *
 * This function processes heredoc redirection (<<),
	reading input until the specified
 * delimiter is encountered. The collected content is stored in a pipe,
	which is then

	* connected to the STDIN of the command. It supports variable expansion
	within the

	* heredoc content based on the expand_vars flag. The function saves the
	original
 * STDIN to restore it after executing the command.
 *
 * For multiple chained heredocs, they are processed in the order they appear
 * in the command line,
	and only the last heredoc's content is connected to STDIN.
 *
 * @param node The heredoc redirection node containing the delimiter string

	* @param env A pointer to the environment variables list used for variable
	expansion
 * @return The exit status of the command after redirection,
	or 1 if an error occurs
 */
static int	handle_here_doc(t_node *node, t_list **env)
{
	t_redirect	*redirect;
	t_node		*cmd_node;
	int			fd;
	int			saved_fd;
	int			status;
	int			heredoc_count;
	int			i;
	t_node		*heredoc_nodes[100];

	heredoc_count = 0;
	cmd_node = node;
	while (cmd_node && cmd_node->type == NODE_HERE_DOC)
	{
		heredoc_nodes[heredoc_count++] = cmd_node;
		cmd_node = cmd_node->left;
	}
	for (i = heredoc_count - 1; i >= 0; i--)
	{
		redirect = (t_redirect *)heredoc_nodes[i]->data;
		fd = handle_heredoc(redirect->filename, *env, redirect->expand_vars);
		if (i != 0)
		{
			if (fd != -1)
				close(fd);
		}
		else
		{
			if (fd == -1)
				return (1);
		}
	}
	saved_fd = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	// TODO: change ternary
	status = cmd_node ? execute_node(cmd_node, env) : 0;
	dup2(saved_fd, STDIN_FILENO);
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
