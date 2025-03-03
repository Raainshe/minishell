/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:33:01 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/16 18:17:01 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_writebase(size_t *remainder, int i, int *chr_written)
{
	char	*base;

	base = "0123456789abcdef";
	while (i > 0)
	{
		i--;
		if (write(1, &base[remainder[i]], 1) == -1)
			return (-1);
		(*chr_written)++;
	}
	return (1);
}

int	ft_print_p(size_t ptr)
{
	int		i;
	size_t	remainder[16];
	int		chr_written;

	i = 0;
	if (ptr == 0)
		return (write(1, "0x0", 3));
	while (ptr > 0)
	{
		remainder[i] = ptr % 16;
		ptr /= 16;
		i++;
	}
	chr_written = 2;
	if (write(1, "0x", 2) == -1)
		return (-1);
	if (ft_writebase(remainder, i, &chr_written) == -1)
		return (-1);
	return (chr_written);
}
