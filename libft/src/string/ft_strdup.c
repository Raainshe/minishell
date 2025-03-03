/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:26:57 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/14 18:21:18 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Allocates sufficient memory for a copy of the string s1 and does the copy
Return
A pointer to it
If insufficient memory is available, NULL is returned
And errno is set to ENOMEM
*/
char	*ft_strdup(const char *s1)
{
	char	*res;
	size_t	len;
	size_t	i;

	len = ft_strlen(s1);
	res = (char *)ft_calloc((len + 1), sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = s1[i];
		i++;
	}
	return (res);
}
