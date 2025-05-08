/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:18:42 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/03 00:11:42 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_banner(void)
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
	ft_putendl_fd("by ksinn & rmakoni \n\n \033[0m", 1);
}

/**
 * @brief Checks if a string contains only whitespace characters
 *
 * Verifies whether the given string consists entirely of spaces,
 * tabs, and other whitespace characters.
 *
 * @param str The string to check
 * @return true if string contains only spaces, false otherwise
 */
bool	is_only_spaces(char *str)
{
	char	pwd[PATH_MAX + 3];
	char	*input;
	char	*line;

	getcwd(pwd, PATH_MAX);
	ft_strlcat(pwd, "> ", PATH_MAX + 3);
	*is_from_pipe = !isatty(fileno(stdin));
	if (!*is_from_pipe)
		input = readline(pwd);
	else
	{
		line = get_next_line(fileno(stdin));
		if (!line)
			return (NULL);
		input = ft_strtrim(line, "\n");
		free(line);
	}
	return (input);
}

int	main(void)
{
	int		*exit_code;
	char	pwd[PATH_MAX + 3];
	t_list	*env;

	print_banner();
	env = init_shell_environment(&exit_code);
	while (42)
	{
		if (!shell_iteration(&env, exit_code, pwd))
			break ;
	}
	return (free_gc(), *t_main.exit_code);
}
