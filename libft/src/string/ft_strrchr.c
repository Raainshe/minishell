/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:06:30 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/11 14:36:33 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Locates the last occurrence of c (converted to a char)
in the string pointed to by s
The terminating null character is considered to be part of the string
Return
A pointer to the located character
or NULL if the character does not appear in the string
*/
char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*temp;

	i = 0;
	temp = (char *)s;
	while (s[i])
	{
		temp++;
		i++;
	}
	while (i >= 0)
	{
		if (*temp == (char)c)
			return (temp);
		temp--;
		i--;
	}
	return (NULL);
}
