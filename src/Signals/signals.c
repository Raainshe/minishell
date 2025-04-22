/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:26:06 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/22 12:52:06 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

/**
 * @brief Handles SIGINT signal in interactive mode
 * @param signum The signal number (always SIGINT)
 *
 * This function sets the global signal flag, writes a newline to stderr,
 * and resets the readline state to prepare for new input.
 */
static void	handle_sigint_interactive(int signum)
{
	g_signal_received = signum;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Handles SIGINT signal in non-interactive mode
 * @param signum The signal number (always SIGINT)
 *
 * This function sets the global signal flag and writes a newline to stderr.
 */
static void	handle_sigint_noninteractive(int signum)
{
	g_signal_received = signum;
	write(STDERR_FILENO, "\n", 1);
}

/**
 * @brief Handles SIGQUIT signal in non-interactive mode
 * @param signum The signal number (always SIGQUIT)
 *
 * This function sets the global signal flag and writes a message to stderr
 * indicating a core dump.
 */
static void	handle_sigquit_noninteractive(int signum)
{
	g_signal_received = signum;
	write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}

/**
 * @brief Sets up signal handlers for interactive mode
 *
 * This function configures SIGINT to be handled by handle_sigint_interactive
 * and ignores SIGQUIT.
 */
void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_interactive;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * @brief Sets up signal handlers for non-interactive mode
 *
 * This function configures SIGINT to be handled by handle_sigint_noninteractive
 * and SIGQUIT to be handled by handle_sigquit_noninteractive.
 */
void	setup_noninteractive_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_noninteractive;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handle_sigquit_noninteractive;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
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
