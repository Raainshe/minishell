/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_x.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:39:44 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/17 16:11:33 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_writebase(int *remainder, int i, int *chr_written, int isX)
{
	char	*base;
	char	c;

	base = "0123456789abcdef";
	while (i > 0)
	{
		i--;
		if (isX)
		{
			c = ft_toupper(base[remainder[i]]);
			if (write(1, &c, 1) == -1)
				return (-1);
			(*chr_written)++;
		}
		else
		{
			if (write(1, &base[remainder[i]], 1) == -1)
				return (-1);
			(*chr_written)++;
		}
	}
	return (1);
}

int	ft_print_x(unsigned int nbr, int isX)
{
	int	i;
	int	remainder[8];
	int	chr_written;

	chr_written = 0;
	i = 0;
	if (nbr == 0)
		return (write(1, "0", 1));
	while (nbr > 0)
	{
		remainder[i] = nbr % 16;
		nbr /= 16;
		i++;
	}
	if (ft_writebase(remainder, i, &chr_written, isX) == -1)
		return (-1);
	return (chr_written);
}
