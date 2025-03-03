/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:49:32 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/14 18:22:45 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Allocates (with malloc(3)) and returns a substring from the string ’s’
The substring begins at index ’start’ and is of maximum size ’len’
Return
The substring
NULL if the allocation fails
*/
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	i;

	if (ft_strlen(s) - start < len)
		len = ft_strlen(s) - start;
	if (start >= ft_strlen(s))
		len = 0;
	res = (char *)ft_calloc((len + 1), sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
