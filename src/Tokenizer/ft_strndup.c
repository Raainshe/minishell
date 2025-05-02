/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:33:33 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/03 00:22:17 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Duplicates at most n characters from the string s
 *
 * Creates a new string by copying at most n characters from s.
 * Always NUL-terminates the copied string regardless of input length.
 * Uses garbage collection for memory management.
 *
 * @param s The source string to duplicate
 * @param n Maximum number of characters to copy
 * @return New duplicated string or NULL if allocation fails
 */
char	*ft_strndup(const char *s, size_t n)
{
	char	*result;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = 0;
	while (s[len] && len < n)
		len++;
	result = (char *)gc_malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = s[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
