/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:53:14 by rmakoni           #+#    #+#             */
/*   Updated: 2025/05/12 12:50:01 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles SIGINT signal in heredoc mode
 * @param signum The signal number (always SIGINT)
 *
 * This function sets the global signal flag, closes file descriptors 3-20
 * (to be thorough in cleanup), and exits the heredoc process with status 1.
 */
void	handle_sigint_heredoc(int signum)
{
	int	i;

	g_signal_received = signum;
	i = 3;
	while (i < 20)
	{
		close(i);
		i++;
	}
	exit(1);
}

/**
 * @brief Handles SIGQUIT signal in non-interactive mode
 * @param signum The signal number (always SIGQUIT)
 *
 * This function sets the global signal flag and writes a message to stderr
 * indicating a core dump.
 */
void	handle_sigquit_noninteractive(int signum)
{
	g_signal_received = signum;
	write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}

/**
 * @brief Resets signal handlers to their default behavior
 *
 * This function restores the default handlers for SIGINT and SIGQUIT.
 */
void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * @brief Resets the terminal and readline state after an interruption
 *
 * This function should be called after a SIGINT in heredoc mode
 * to ensure the terminal and readline library are properly reset.
 */
void	reset_term_after_signal(void)
{
	struct termios	term;

	rl_free_line_state();
	rl_cleanup_after_signal();
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ECHOCTL | ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	setup_interactive_signals();
}

/**
 * @brief Retrieves the status code based on the last received signal
 * @return The status code: 130 for SIGINT, 131 for SIGQUIT, or 0 otherwise
 *
 * This function checks the global signal flag, determines the appropriate
 * status code, and resets the flag.
 */
int	get_signal_status(void)
{
	int	status;

	if (g_signal_received == SIGINT)
		status = 130;
	else if (g_signal_received == SIGQUIT)
		status = 131;
	else
		status = 0;
	g_signal_received = 0;
	return (status);
}
