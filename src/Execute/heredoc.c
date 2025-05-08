/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:49:49 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 11:53:22 by ksinn            ###   ########.fr       */
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
 * @brief Collects all heredoc nodes in a chain
 * @param node The starting heredoc node
 * @param heredoc_nodes Array to store the heredoc nodes
 * @return Number of heredoc nodes found and the command node
 */
static int	collect_heredoc_nodes(t_node *node, t_node *heredoc_nodes[100],
		t_node **cmd_node)
{
	int	heredoc_count;

	heredoc_count = 0;
	*cmd_node = node;
	while (*cmd_node && (*cmd_node)->type == NODE_HERE_DOC)
	{
		heredoc_nodes[heredoc_count++] = *cmd_node;
		*cmd_node = (*cmd_node)->left;
	}
	return (heredoc_count);
}

/**
 * @brief Process all heredoc inputs in a chain
 * @param heredoc_nodes Array of heredoc nodes
 * @param heredoc_count Number of heredoc nodes
 * @param env Environment variables list
 * @return File descriptor for reading from the last heredoc, or -1 on error
 */
static int	process_heredocs(t_node *heredoc_nodes[100], int heredoc_count,
		t_list *env)
{
	t_redirect	*redirect;
	int			fd;
	int			i;

	i = heredoc_count - 1;
	while (i >= 0)
	{
		redirect = (t_redirect *)heredoc_nodes[i]->data;
		fd = handle_heredoc(redirect->filename, env, redirect->expand_vars);
		if (i != 0)
		{
			if (fd != -1)
				close(fd);
		}
		else if (fd == -1)
			return (-1);
		i--;
	}
	return (fd);
}

/**
 * @brief Execute a command with stdin redirected from a heredoc
 * @param cmd_node The command node to execute
 * @param env Environment variables list
 * @param fd File descriptor to read from
 * @return Exit status of the command
 */
static int	execute_with_heredoc_input(t_node *cmd_node, t_list **env, int fd)
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
 *
 * @param node The heredoc redirection node containing the delimiter string

	* @param env A pointer to the environment variables list used for variable
	expansion
 * @return The exit status of the command after redirection,
	or 1 if an error occurs
 */
int	handle_here_doc(t_node *node, t_list **env)
{
	t_node	*heredoc_nodes[100];
	t_node	*cmd_node;
	int		heredoc_count;
	int		fd;

	heredoc_count = collect_heredoc_nodes(node, heredoc_nodes, &cmd_node);
	fd = process_heredocs(heredoc_nodes, heredoc_count, *env);
	if (fd == -1)
		return (1);
	return (execute_with_heredoc_input(cmd_node, env, fd));
}
