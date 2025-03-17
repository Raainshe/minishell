/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_free_context.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:31:32 by rmakoni           #+#    #+#             */
/*   Updated: 2025/03/17 13:01:04 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/*
** Safely frees all memory associated with a specific context
** This allows for more granular memory management than free_gc()
*/
void	gc_free_context(t_gc_context context)
{
	t_list	**collections;
	t_list	*current;
	t_list	*next;

	if (context < 0 || context >= GC_ARR_SIZE)
		return ;
	collections = gc_holder();
	if (!collections || !collections[context])
		return ;
	current = collections[context];
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
	collections[context] = NULL;
}
