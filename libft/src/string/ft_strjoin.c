/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:16:48 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/14 18:21:11 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Allocates (with malloc(3)) and returns a new string
which is the result of the concatenation of ’s1’ and ’s2’.
Return
The new string
NULL if the allocation fails
*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	res = (char *)ft_calloc((s1_len + s2_len + 1), sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < s1_len + s2_len)
	{
		if (i < s1_len)
			res[i] = s1[i];
		if (i < s2_len)
			res[s1_len + i] = s2[i];
		i++;
	}
	return (res);
}
