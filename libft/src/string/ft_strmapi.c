/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:02:52 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/14 18:21:06 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Applies the function f to each character of the string s
passing its index as the first argument and the character itself as the second
A new string is created (using malloc(3))
to collect the results from the successive applications of f
Return
The string created from the successive applications of ’f’
NULL if the allocation fails
*/
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*res;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	res = (char *)ft_calloc((len + 1), sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (s[i])
	{
		res[i] = (*f)(i, s[i]);
		i++;
	}
	return (res);
}
