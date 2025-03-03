/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:17:50 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/11 14:32:10 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Converts a lower-case letter to the corresponding upper-case letter
Return
the corresponding upper-case letter if there is one
*/
int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= ('a' - 'A');
	return (c);
}
