/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:43:32 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/17 13:02:02 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/*
** Frees all memory tracked by the garbage collector
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
				free(current);
				current = next;
			}
			collections[i] = NULL;
		}
		i++;
	}
}

/*
** Allocates memory using malloc
** If allocation fails, frees all garbage collected memory before returning NULL
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

/*
** Adds a pointer to a specific context for tracking
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
