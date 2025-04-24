/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:53:14 by rmakoni           #+#    #+#             */
/*   Updated: 2025/04/24 13:37:43 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
