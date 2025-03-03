/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:19:25 by ksinn             #+#    #+#             */
/*   Updated: 2024/10/16 18:22:12 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_getcase(char c, int *res, va_list args)
{
	if (c == 'c')
		*res = ft_putchar_fd(va_arg(args, int), 1);
	else if (c == 's')
		*res = ft_print_s(va_arg(args, char *));
	else if (c == 'p')
		*res = ft_print_p(va_arg(args, size_t));
	else if (c == 'd' || c == 'i')
		*res = ft_putnbr_fd(va_arg(args, int), 1);
	else if (c == 'u')
		*res = ft_print_u(va_arg(args, unsigned int));
	else if (c == 'x' || c == 'X')
		*res = ft_print_x(va_arg(args, unsigned int), c == 'X');
	else if (c == '%')
		*res = ft_putchar_fd('%', 1);
	else
		return (-1);
	return (1);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	size_t	chr_printed;
	int		res;

	va_start(args, str);
	chr_printed = 0;
	while (*str)
	{
		if (*str == '%')
		{
			if (ft_getcase(*(++str), &res, args) == -1)
				return (chr_printed);
			if (res == -1)
				return (-1);
			chr_printed += res;
		}
		else
		{
			if (ft_putchar_fd(*str, 1) == -1)
				return (-1);
			chr_printed++;
		}
		str++;
	}
	return (va_end(args), chr_printed);
}
