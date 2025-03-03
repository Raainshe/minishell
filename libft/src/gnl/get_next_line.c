/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:11:05 by ksinn             #+#    #+#             */
/*   Updated: 2024/11/08 14:25:31 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_read_and_fill(int fd, char *buffer, char *buff)
{
	int	bytes_read;

	bytes_read = read(fd, buff, BUFFER_SIZE);
	if (bytes_read == -1)
	{
		free(buffer);
		return (free(buff), NULL);
	}
	while (bytes_read > 0)
	{
		buff[bytes_read] = '\0';
		buffer = gnl_ft_strjoin(buffer, buff);
		if (!buffer)
			return (free(buff), NULL);
		if (gnl_ft_strchr(buffer, '\n'))
			break ;
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			return (free(buff), NULL);
		}
	}
	return (free(buff), buffer);
}

static char	*ft_fill_buffer(int fd, char *buffer)
{
	char	*buff;

	if (!buffer)
	{
		buffer = (char *)gnl_ft_calloc(1, sizeof(char));
		if (!buffer)
			return (NULL);
	}
	if (gnl_ft_strchr(buffer, '\n'))
		return (buffer);
	buff = (char *)gnl_ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buff)
		return (free(buffer), NULL);
	return (ft_read_and_fill(fd, buffer, buff));
}

static char	*ft_create_line(char *buffer, size_t len, int has_nl)
{
	char	*str;
	size_t	i;

	if (has_nl)
		str = (char *)gnl_ft_calloc(len + 2, sizeof(char));
	else
		str = (char *)gnl_ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		str[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
	{
		str[i] = buffer[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static char	*ft_getline(char *buffer)
{
	size_t	i;
	int		has_nl;

	i = 0;
	if (!buffer[0])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	has_nl = (buffer[i] == '\n');
	return (ft_create_line(buffer, i, has_nl));
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buffer;

	if (fd == -1)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = ft_fill_buffer(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_getline(buffer);
	if (!line)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	buffer = ft_trimbuffer(buffer);
	return (line);
}
