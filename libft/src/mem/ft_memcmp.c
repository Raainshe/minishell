/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:10:32 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/11 14:42:37 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Compares byte string s1 against byte string s2
Both strings are assumed to be n bytes long
Return
zero if the two strings are identical
otherwise returns the difference between the first two differing bytes
(treated as unsigned char values)
*/
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (n > 0)
	{
		if (*str1 != *str2)
			return ((*str1) - *(str2));
		str1++;
		str2++;
		n--;
	}
	return (0);
}
