/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:24:13 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/15 12:59:42 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Outputs the character ’c’ to the given file descriptor
*/
int	ft_putchar_fd(char c, int fd)
{
	return (write(fd, &c, 1));
}
