/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:46:12 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/26 15:01:33 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_node(t_node *node, char **env)
{
	int	status;

	status = 0;
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		status = execute_command(node, env);
	else if (node->type == NODE_PIPE)
		status = execute_pipe(node, env);
	else if (node->type == NODE_REDIRECT_IN || node->type == NODE_REDIRECT_OUT
		|| node->type == NODE_APPEND)
		status = execute_redirect(node, env);
	return (status);
}
