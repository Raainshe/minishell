/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:29:41 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/14 18:22:50 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_countstr(char const *s, char c)
{
	size_t	nbr_str;

	nbr_str = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
			nbr_str++;
		while (*s && *s != c)
			s++;
	}
	return (nbr_str);
}

static char	*ft_getstr(const char *s, char c)
{
	char	*res;
	size_t	len;
	size_t	i;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	res = (char *)ft_calloc((len + 1), sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = s[i];
		i++;
	}
	return (res);
}

static void	ft_free_res(char **res, size_t i)
{
	while (i > 0)
		free(res[--i]);
	free(res);
}

/*
Allocates (with malloc(3)) and returns an array of strings
obtained by splitting ’s’ using the character ’c’ as a delimiter.
The array must end with a NULL pointer
Return
The array of new strings resulting from the split
NULL if the allocation fails
*/
char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	nbr_str;
	size_t	i;

	nbr_str = ft_countstr(s, c);
	res = (char **)ft_calloc((nbr_str + 1), sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (i < nbr_str)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			res[i] = ft_getstr(s, c);
			if (!(res[i]))
				return (ft_free_res(res, i), NULL);
		}
		while (*s && *s != c)
			s++;
		i++;
	}
	return (res);
}
