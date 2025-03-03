/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_u.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:47:34 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/16 15:14:47 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_u(unsigned int nbr)
{
	char	result[11];
	int		i;

	if (nbr == 0)
		return (write(1, "0", 1));
	i = 10;
	result[i] = '\0';
	while (nbr > 0)
	{
		result[--i] = nbr % 10 + '0';
		nbr /= 10;
	}
	return (write(1, &result[i], 11 - i - 1));
}
