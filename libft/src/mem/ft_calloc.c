/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:35:38 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/08 13:32:00 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "libft.h"

/*
Contiguously allocates enough space for count objects
that are size bytes of memory each
The allocated memory is filled with bytes of value zero
Return
A pointer to the allocated memory
If there is an error, they return a NULL pointer and set errno to ENOMEM
*/
void	*ft_calloc(size_t count, size_t size)
{
	void	*obj;

	obj = gc_malloc(count * size * sizeof(char));
	if (!obj)
		return (NULL);
	ft_bzero(obj, size * count);
	return (obj);
}
