/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect_helper_helper_helper.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:45:00 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/13 13:31:19 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Prints an error message for redirection failures.
 *
 * Displays a formatted error to `STDERR_FILENO`:
 * "minishell: [filename]: [error_description]".
 * Used when a file redirection operation fails.
 *
 * @param redirect Ptr to `t_redirect` with the problematic `filename`.
 * @return Always returns 1 (error).
 */
int	print_redirect_error(t_redirect *redirect)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(redirect->filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	return (1);
}

/**
 * @brief Handles heredoc input in the child process.
 *
 * Sets heredoc signal handlers. Reads lines using `readline` until `delimiter`
 * is matched or EOF (Ctrl+D) is received. Input lines are written to `pipe_fd
 * [1]`.
 * Variables are expanded if `expand_vars` is true.
 * Exits 0 on success, 130 on EOF/Ctrl+D.
 *
 * @param pipe_fd Pipe FDs. Child writes to `pipe_fd[1]`.
 * @param delimiter String ending heredoc.
 * @param env Environment variables for expansion.
 * @param expand_vars If true, expand variables in heredoc.
 */
void	handle_heredoc_child(int pipe_fd[2], char *delimiter, t_list *env,
		bool expand_vars)
{
	t_proc_heredoc_line	phl;
	int					saved_stdin;

	phl.pipe_fd = pipe_fd[1];
	phl.env = env;
	phl.expand_vars = expand_vars;
	phl.delimiter = delimiter;
	setup_heredoc_signals();
	close(pipe_fd[0]);
	phl.tty_fd = setup_heredoc_terminal();
	if (phl.tty_fd == -1)
		clean_and_exit(pipe_fd[1], -1, 1);
	while (1)
	{
		saved_stdin = redirect_stdin_to_tty(phl.tty_fd);
		if (saved_stdin == -1)
			clean_and_exit(pipe_fd[1], phl.tty_fd, 1);
		phl.line = readline("> ");
		restore_stdin(saved_stdin);
		if (!phl.line)
			break ;
		process_heredoc_line(&phl);
	}
	clean_and_exit(pipe_fd[1], phl.tty_fd, 130);
}
