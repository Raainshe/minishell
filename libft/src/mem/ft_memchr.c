/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:42:54 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/11 14:41:06 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Locates the first occurrence of c (converted to an unsigned char) in string s
within n bytes
Return
a pointer to the byte located, or NULL if no such byte exists within n bytes
*/
void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*str;
	unsigned char		cc;

	str = s;
	cc = (unsigned char)c;
	if (n == 0)
		return (NULL);
	while (n > 0)
	{
		if ((*str) == cc)
			return ((void *)str);
		str++;
		n--;
	}
	return (NULL);
}
