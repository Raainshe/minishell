/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlower.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:48:01 by rmakoni           #+#    #+#             */
/*   Updated: 2025/03/26 15:26:53 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "libft.h"

/*
 * Creates a new string with all characters converted to lowercase.
 * Original string remains unchanged.
 *
 * @param str String to convert (can be NULL)
 * @return New malloc'd string in lowercase, or NULL if str is NULL/malloc fails
 *
 * Example:
 * ft_strlower("Hello") // returns "hello"
 */
char	*ft_strlower(char *str)
{
	char	*lower_str;
	size_t	s_len;
	int		i;

	if (!str)
		return (NULL);
	s_len = ft_strlen(str);
	lower_str = gc_malloc(s_len + 1);
	if (!lower_str)
		return (NULL);
	gc_add_context(EXECUTOR, lower_str);
	i = 0;
	while (str[i] != '\0')
	{
		lower_str[i] = ft_tolower(str[i]);
		i++;
	}
	lower_str[i] = '\0';
	return (lower_str);
}
