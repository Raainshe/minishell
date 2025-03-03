/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:14:00 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/11 14:57:12 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Applies the function ’f’ on each character of the string passed as argument
passing its index as first argument
Each character is passed by address to ’f’ to be modified if necessary
*/
void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	i = 0;
	while (*s)
	{
		(*f)(i, s);
		i++;
		s++;
	}
}
