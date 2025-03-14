/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_free_context.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:31:32 by rmakoni           #+#    #+#             */
/*   Updated: 2025/03/14 17:32:27 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

void	gc_free_context(t_gc_context context)
{
	t_list **collections;

	collections = gc_holder();
	ft_lstclear(collections[context], free);
	collections[context] = NULL;
}