/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:18:42 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/08 13:07:07 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
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
	ft_putendl_fd("by rmakoni & ksinn\n\n \033[0m", 1);
}

static bool	is_only_spaces(char *str)
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

/**
 * Initialize shell environment and settings
 * Returns a pointer to the environment list
 */
static t_list	*init_shell(int **exit_code)
{
	t_list	*env;

	print_banner();
	env = copy_environ(environ);
	*exit_code = ft_exit_code_holder();
	setup_interactive_signals();
	return (env);
}

/**
 * Get user input from terminal or pipe
 * Returns the input string or NULL if EOF
 */
static char	*get_input(bool *is_from_pipe)
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

/**
 * Parse input string into an AST
 * Returns the root node of the AST or NULL on error
 */
static t_node	*parse_input(char *input, t_list *env)
{
	char	**token_strings;
	t_token	*tokens;
	t_node	*ast;

	token_strings = ft_split_tokens(input);
	free(input);
	if (!token_strings)
		return (NULL);
	tokens = ft_tokenize(token_strings, env);
	if (!tokens)
		return (NULL);
	ast = parse_tokens(tokens);
	return (ast);
}

int	main(void)
{
	char	*input;
	t_node	*ast;
	int		*exit_code;
	t_list	*env;
	bool	is_from_pipe;

	env = init_shell(&exit_code);
	while (42)
	{
		input = get_input(&is_from_pipe);
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if ((input[0] == '\0' || is_only_spaces(input)) && !is_from_pipe)
		{
			free(input);
			continue ;
		}
		if (!is_from_pipe)
			add_history(input);
		ast = parse_input(input, env);
		if (!ast)
			continue ;
		setup_noninteractive_signals();
		*exit_code = execute_node(ast, &env);
		if (g_signal_received)
		{
			*exit_code = get_signal_status();
			reset_term_after_signal();
		}
		else
		{
			setup_interactive_signals();
		}
		gc_free_context(TOKENIZER);
		gc_free_context(AST);
		gc_free_context(EXECUTOR);
	}
	free_gc();
	return (*exit_code);
}
