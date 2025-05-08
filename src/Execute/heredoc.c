/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:35:40 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/08 16:38:33 by ksinn            ###   ########.fr       */
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
 * @brief Collects all heredoc nodes into an array
 * @param node The starting node
 * @param heredoc_nodes Array to store the collected heredoc nodes
 * @return The command node after all heredoc nodes
 */
static t_node	*collect_heredoc_nodes(t_node *node, t_node *heredoc_nodes[100],
		int *heredoc_count)
{
	t_node	*cmd_node;

	*heredoc_count = 0;
	cmd_node = node;
	while (cmd_node && cmd_node->type == NODE_HERE_DOC)
	{
		heredoc_nodes[(*heredoc_count)++] = cmd_node;
		cmd_node = cmd_node->left;
	}
	return (cmd_node);
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
	t_redirect	*redirect;
	int			fd;
	int			i;

	i = heredoc_count - 1;
	while (i >= 0)
	{
		redirect = (t_redirect *)heredoc_nodes[i]->data;
		fd = handle_heredoc(redirect->filename, *env, redirect->expand_vars);
		if (fd == -1)
		{
			if (g_signal_received == SIGINT)
				return (-130);
			return (-1);
		}
		if (i != 0)
			close(fd);
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
	t_node	*cmd_node;
	int		heredoc_count;
	t_node	*heredoc_nodes[100];
	int		fd;

	cmd_node = collect_heredoc_nodes(node, heredoc_nodes, &heredoc_count);
	fd = process_heredocs(heredoc_nodes, heredoc_count, env);
	if (fd < 0)
	{
		if (fd == -130)
			return (130);
		return (1);
	}
	return (execute_with_heredoc(cmd_node, env, fd));
}
