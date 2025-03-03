/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:11:08 by ksinn             #+#    #+#             */
/*   Updated: 2024/11/08 14:27:11 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*gnl_ft_calloc(size_t count, size_t size)
{
	void	*obj;
	char	*str;
	size_t	i;

	obj = malloc(count * size * sizeof(char));
	if (!obj)
		return (NULL);
	str = obj;
	i = 0;
	while (i < size * count)
	{
		str[i] = 0;
		i++;
	}
	return (obj);
}

char	*gnl_ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*gnl_ft_strjoin(char *s1, char *s2)
{
	char	*res;
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;

	if (!s1 || !s2)
	{
		if (s1)
			free(s1);
		return (NULL);
	}
	s1_len = gnl_ft_strlen(s1);
	s2_len = gnl_ft_strlen(s2);
	res = (char *)gnl_ft_calloc((s1_len + s2_len + 1), sizeof(char));
	if (!res)
		return (free(s1), NULL);
	i = -1;
	while (++i < s1_len + s2_len)
	{
		if (i < s1_len)
			res[i] = s1[i];
		if (i < s2_len)
			res[s1_len + i] = s2[i];
	}
	return (free(s1), res);
}

size_t	gnl_ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
	{
		len++;
	}
	return (len);
}

char	*ft_trimbuffer(char *buffer)
{
	size_t	i;
	size_t	j;
	char	*new_buffer;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
		return (free(buffer), NULL);
	new_buffer = (char *)gnl_ft_calloc(gnl_ft_strlen(buffer) - i, sizeof(char));
	if (!new_buffer)
		return (free(buffer), NULL);
	i++;
	j = 0;
	while (buffer[i])
		new_buffer[j++] = buffer[i++];
	new_buffer[j] = '\0';
	return (free(buffer), new_buffer);
}
