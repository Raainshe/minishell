/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:43:32 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/27 10:40:19 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/**
 * @brief Frees all memory in a specific collection
 *
 * Iterates through the linked list of a collection and frees
 * each node and its content, then sets the collection pointer to NULL.
 *
 * @param collections Array of collections
 * @param index Index of the collection to free
 * @return None (void function)
 */
static void	free_collection(t_list **collections, int index)
{
	t_list	*current;
	t_list	*next;

	if (!collections || index < 0 || index >= GC_ARR_SIZE
		|| !collections[index])
		return ;
	current = collections[index];
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
	collections[index] = NULL;
}

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
	int		i;

	collections = gc_holder();
	if (!collections)
		return ;
	i = 0;
	while (i < GC_ARR_SIZE)
	{
		if (collections[i])
		{
			free_collection(collections, i);
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
