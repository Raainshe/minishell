/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:43:32 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/14 17:38:00 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

void	free_gc(void)
{
	t_list	**collections;
	int		i;

	collections = gc_holder();
	i = 0;
	while (i < GC_ARR_SIZE)
	{
		ft_lstclear(collections[i], free);
		collections[i] = NULL;
		i++;
	}
}

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

void	gc_add_context(t_gc_context context, void *ptr)
{
	t_list **collections;

	collections = gc_holder();
	if (!collections[context])
		collections[context] = ft_lstnew(ptr);
	else
		ft_lstadd_back(collections[context], ft_lstnew(ptr));
}