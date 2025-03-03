/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:39:33 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/11 14:32:55 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Converts an upper-case letter to the corresponding lower-case letter
Return
The corresponding lower-case letter if there is one
*/
int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += ('a' - 'A');
	return (c);
}
