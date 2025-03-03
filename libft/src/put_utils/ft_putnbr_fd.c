/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:38:53 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/16 15:11:11 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_negative(long *nbr, int *sign)
{
	if (*nbr < 0)
	{
		*sign = 1;
		*nbr = -(*nbr);
	}
}

/*
Outputs the integer ’n’ to the given file descriptor
*/
int	ft_putnbr_fd(int n, int fd)
{
	char	result[12];
	long	nbr;
	int		sign;
	int		i;

	nbr = n;
	if (nbr == 0)
		return (write(fd, "0", 1));
	sign = 0;
	ft_negative(&nbr, &sign);
	i = 11;
	result[i] = '\0';
	while (nbr > 0)
	{
		result[--i] = nbr % 10 + '0';
		nbr /= 10;
	}
	if (sign)
		result[--i] = '-';
	return (write(fd, &result[i], 12 - i - 1));
}
