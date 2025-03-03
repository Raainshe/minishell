/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:17:26 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/14 18:26:41 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_nbrlen(int nbr, size_t *sign)
{
	size_t	len;

	*sign = 0;
	len = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
	{
		*sign = 1;
		len++;
	}
	while (nbr != 0)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

/*
Allocates (with malloc(3)) and returns a string
representing the integer received as an argument
Negative numbers must be handled
Return
The string representing the integer
NULL if the allocation fails
*/
char	*ft_itoa(int n)
{
	char	*res;
	size_t	len;
	size_t	sign;
	long	nbr;

	len = ft_nbrlen(n, &sign);
	res = (char *)ft_calloc((len + 1), sizeof(char));
	if (!res)
		return (NULL);
	nbr = n;
	if (n < 0)
		nbr = -nbr;
	while (len > sign)
	{
		res[--len] = ((nbr % 10) + '0');
		nbr /= 10;
	}
	if (sign)
		res[0] = '-';
	return (res);
}
