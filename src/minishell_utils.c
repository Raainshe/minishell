/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:33:07 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/08 13:33:33 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_banner(void)
{
	ft_putstr_fd("\033[0;34m", 1);
	ft_putendl_fd(" _________________________________________________________",
		1);
	ft_putendl_fd("|  _____            _           _                 _       |",
		1);
	ft_putendl_fd("| |_   _|          | |         | |               | |      |",
		1);
	ft_putendl_fd("|   | |  __ _  ___ | |__   ___ | | ___  _ __   __| | ___  |",
		1);
	ft_putstr_fd("|   | | / _` |/ _ \\| '_ \\ / _ \\| ", 1);
	ft_putendl_fd("|/ _ \\| '_ \\ / _` |/ _ \\ |", 1);
	ft_putendl_fd("|  _| || (_| | (_) | |_) | (_) | | (_) | | | | (_| | (_) ||",
		1);
	ft_putstr_fd("| |_____\\__, |\\___/|_.__/ ", 1);
	ft_putendl_fd("\\___/|_|\\___/|_| |_|\\__,_|\\___/ |", 1);
	ft_putendl_fd("|        __/ |                                            |",
		1);
	ft_putendl_fd("|       |___/                                             |",
		1);
	ft_putendl_fd("|_________________________________________________________|",
		1);
	ft_putendl_fd("by rmakoni & ksinn\n\n \033[0m", 1);
}

bool	is_only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (false);
		i++;
	}
	return (true);
}
