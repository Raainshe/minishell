/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:11:11 by ksinn             #+#    #+#             */
/*   Updated: 2024/11/08 14:24:24 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
void	*gnl_ft_calloc(size_t count, size_t size);
char	*gnl_ft_strchr(const char *s, int c);
char	*gnl_ft_strjoin(char *s1, char *s2);
size_t	gnl_ft_strlen(const char *s);
char	*ft_trimbuffer(char *buffer);

#endif
