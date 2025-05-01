/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:18:42 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 15:05:28 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	print_banner(void)
// {
// 	ft_putstr_fd("\033[0;34m", 1);
// 	ft_putendl_fd(" _________________________________________________________",
// 		1);
// 	ft_putendl_fd("|  _____            _           _                 _       |",
// 		1);
// 	ft_putendl_fd("| |_   _|          | |         | |               | |      |",
// 		1);
// 	ft_putendl_fd("|   | |  __ _  ___ | |__   ___ | | ___  _ __   __| | ___  |",
// 		1);
// 	ft_putstr_fd("|   | | / _` |/ _ \\| '_ \\ / _ \\| ", 1);
// 	ft_putendl_fd("|/ _ \\| '_ \\ / _` |/ _ \\ |", 1);
// 	ft_putendl_fd("|  _| || (_| | (_) | |_) | (_) | | (_) | | | | (_| | (_) ||",
// 		1);
// 	ft_putstr_fd("| |_____\\__, |\\___/|_.__/ ", 1);
// 	ft_putendl_fd("\\___/|_|\\___/|_| |_|\\__,_|\\___/ |", 1);
// 	ft_putendl_fd("|        __/ |                                            |",
// 		1);
// 	ft_putendl_fd("|       |___/                                             |",
// 		1);
// 	ft_putendl_fd("|_________________________________________________________|",
// 		1);
// 	ft_putendl_fd("by rmakoni & ksinn\n\n \033[0m", 1);
// }

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

// TODO: Don't expand delimiters of heredoc
int	main(void)
{
	int		*exit_code;
	char	pwd[PATH_MAX + 3];
	t_list	*env;

	env = init_shell_environment(&exit_code);
	while (42)
	{
		if (!shell_iteration(&env, exit_code, pwd))
			break ;
	}
	free_gc();
	return (*exit_code);
}
