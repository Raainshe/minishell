/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:39:46 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/03 00:22:17 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Returns a pointer to the static exit code variable
 *
 * Provides access to a globally accessible exit code that persists
 * throughout program execution. This allows different parts of the
 * shell to check and set the exit status of commands.
 *
 * @return Pointer to the static exit code integer
 */
int	*ft_exit_code_holder(void)
{
	static int	exit_code = 0;

	return (&exit_code);
}
