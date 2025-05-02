/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_free_context.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:31:32 by rmakoni           #+#    #+#             */
/*   Updated: 2025/04/23 15:25:07 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/**
 * @brief Safely frees all memory associated with a specific context
 *
 * This allows for more granular memory management than free_gc().
 * It iterates through the linked list of the specified context,
 * frees each node's content and the node itself, then sets the
 * context pointer to NULL.
 *
 * @param context The garbage collector context to free
 * @return None (void function)
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
		{
			free(current->content);
			current->content = NULL;
		}
		free(current);
		current = next;
	}
	collections[context] = NULL;
}
