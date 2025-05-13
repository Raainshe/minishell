/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:35:40 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/13 13:15:26 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Processes a single heredoc and returns its file descriptor
 *
 * @param node The heredoc node to process
 * @param env The environment variables list
 * @return The file descriptor, or negative value on error
 */
static int	process_single_heredoc(t_node *node, t_list **env)
{
	t_redirect	*redirect;
	int			fd;

	redirect = (t_redirect *)node->data;
	fd = handle_heredoc(redirect->filename, *env, redirect->expand_vars);
	return (fd);
}

/**
 * @brief Manages file descriptors between heredoc processing
 *
 * @param fd The current file descriptor
 * @param last_fd The previous file descriptor
 * @param is_last Whether this is the last heredoc in the chain
 * @return The file descriptor to be returned or -1 on error
 */
static int	manage_heredoc_fds(int fd, int *last_fd, bool is_last)
{
	if (fd == -1)
		return (handle_heredoc_error(*last_fd));
	if (!is_last)
	{
		if (*last_fd != -1)
			close(*last_fd);
		*last_fd = fd;
		return (0);
	}
	return (fd);
}

/**
 * @brief Processes all heredoc nodes in reverse order
 * @param heredoc_nodes Array of heredoc nodes
 * @param heredoc_count Number of heredoc nodes
 * @param env The environment variables list for expansion
 * @return The file descriptor for the last heredoc, or -1 on error
 */
static int	process_heredocs(t_node *heredoc_nodes[100], int heredoc_count,
		t_list **env)
{
	int		i;
	int		last_fd;
	int		fd;
	int		result;
	bool	is_last;

	i = heredoc_count - 1;
	last_fd = -1;
	fd = -1;
	while (i >= 0)
	{
		fd = process_single_heredoc(heredoc_nodes[i], env);
		is_last = (i == 0);
		result = manage_heredoc_fds(fd, &last_fd, is_last);
		if (result != 0 || is_last)
			return (result);
		i--;
	}
	return (fd);
}

/**
 * @brief Executes a command with heredoc input
 * @param cmd_node The command node to execute
 * @param env The environment variables list
 * @param fd The file descriptor to use as input
 * @return The exit status of the command
 */
static int	execute_with_heredoc(t_node *cmd_node, t_list **env, int fd)
{
	int	saved_fd;
	int	status;

	saved_fd = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	if (cmd_node)
		status = execute_node(cmd_node, env);
	else
		status = 0;
	dup2(saved_fd, STDIN_FILENO);
	close(saved_fd);
	return (status);
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
 * If SIGINT (Ctrl+C) is received during any heredoc,
	all remaining heredocs are skipped.
 *
 * @param node The heredoc redirection node containing the delimiter string
 * @param env A pointer to the environment variables list used for variable
	expansion
 * @return The exit status of the command after redirection,
	or 1 if an error occurs
 */
int	handle_here_doc(t_node *node, t_list **env)
{
	t_node		*cmd_node;
	int			heredoc_count;
	t_node		*heredoc_nodes[100];
	int			fd;
	t_redirect	*redirect;

	redirect = (t_redirect *)node->data;
	if (redirect->heredoc_fd != -1)
	{
		cmd_node = collect_heredoc_nodes(node, heredoc_nodes, &heredoc_count);
		fd = redirect->heredoc_fd;
		return (execute_with_heredoc(cmd_node, env, fd));
	}
	cmd_node = collect_heredoc_nodes(node, heredoc_nodes, &heredoc_count);
	fd = process_heredocs(heredoc_nodes, heredoc_count, env);
	if (fd < 0)
	{
		if (fd == -130)
		{
			close_heredoc_fds(heredoc_count);
			return (130);
		}
		return (1);
	}
	return (execute_with_heredoc(cmd_node, env, fd));
}
