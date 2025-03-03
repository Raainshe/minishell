/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:49:32 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/11 14:30:51 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Appends string src to the end of dst
It will append at most dstsize - strlen(dst) - 1 characters
It will then NUL-terminate, unless dstsize is 0
or the original dst string was longer than dstsize
If the src and dst strings overlap, the behavior is undefined
Return
the total length of the string they tried to create
the initial length of dst plus the length of src
If the return value is >= dstsize, the output string has been truncated
It is the caller's responsibility to handle this
*/
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = 0;
	while (dst[dst_len] && dst_len < dstsize)
		dst_len++;
	src_len = 0;
	while (src[src_len])
		src_len++;
	if (dst_len == dstsize)
		return (dstsize + src_len);
	i = 0;
	while (src[i] && dst_len < dstsize - i - 1)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	if (dst_len + i < dstsize)
		dst[dst_len + i] = '\0';
	else
		dst[dst_len - 1] = '\0';
	return (dst_len + src_len);
}
