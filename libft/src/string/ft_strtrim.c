/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:30:57 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/14 18:22:51 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Allocates (with malloc(3)) and returns a copy of ’s1’
with the characters specified in ’set’
removed from the beginning and the end of the string
Return
The trimmed string
NULL if the allocation fails
*/
char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		left;
	int		right;
	int		i;

	left = 0;
	right = 0;
	while (s1[left] && ft_strchr(set, s1[left]))
		left++;
	if ((long long)ft_strlen(s1) >= 0)
		right = ft_strlen(s1) - 1;
	while (right > left && ft_strchr(set, s1[right]))
		right--;
	res = (char *)ft_calloc((right - left + 2), sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i + left <= right)
	{
		res[i] = s1[i + left];
		i++;
	}
	return (res);
}
