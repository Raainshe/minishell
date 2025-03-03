/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:20:13 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/11 14:16:23 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Check if c is alphabetical character
Return
TRUE	1
FALSE	0
*/
int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}
