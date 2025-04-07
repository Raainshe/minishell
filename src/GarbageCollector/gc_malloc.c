/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:43:32 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/07 17:14:08 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/**
 * @brief Frees all memory tracked by the garbage collector
 *
 * Iterates through all contexts in the garbage collector and frees
 * each linked list node and its content, then sets all context
 * pointers to NULL.
 *
 * @return None (void function)
 */
void	free_gc(void)
{
	t_list	**collections;
	t_list	*current;
	t_list	*next;
	int		i;

	collections = gc_holder();
	if (!collections)
		return ;
	i = 0;
	while (i < GC_ARR_SIZE)
	{
		if (collections[i])
		{
			current = collections[i];
			while (current)
			{
				next = current->next;
				free(current->content);
				current->content = NULL;
				free(current);
				current = next;
			}
			collections[i] = NULL;
		}
		i++;
	}
}

/**
 * @brief Allocates memory using malloc and tracks it with the garbage collector
 *
 * If allocation fails,
	frees all garbage collected memory before returning NULL.
 *
 * @param size Number of bytes to allocate
 * @return Pointer to the allocated memory, or NULL if allocation fails
 */
void	*gc_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		free_gc();
		return (NULL);
	}
	return (ptr);
}

/**
 * @brief Adds a pointer to a specific context for tracking
 *
 * Creates a new linked list node for the pointer and adds it to the
 * specified context's linked list. If the context doesn't exist yet,
 * initializes it with the new node.
 *
 * @param context The context to add the pointer to
 * @param ptr The pointer to track
 * @return None (void function)
 */
void	gc_add_context(t_gc_context context, void *ptr)
{
	t_list	**collections;
	t_list	*new_node;

	if (context < 0 || context >= GC_ARR_SIZE || !ptr)
		return ;
	collections = gc_holder();
	if (!collections)
		return ;
	new_node = ft_lstnew(ptr);
	if (!new_node)
		return ;
	if (!collections[context])
		collections[context] = new_node;
	else
		ft_lstadd_back(&collections[context], new_node);
}
