/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:33:07 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/12 17:20:57 by ksinn            ###   ########.fr       */
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

/**
 * Clean up resources after command execution
 */
void	cleanup_execution_resources(t_node *ast)
{
	close_preprocessed_heredocs(ast);
	gc_free_context(TOKENIZER);
	gc_free_context(AST);
	gc_free_context(EXECUTOR);
}

/**
 * Initialize shell environment and settings
 * Returns a pointer to the environment list
 */
t_list	*init_shell(int **exit_code)
{
	t_list	*env;

	print_banner();
	env = copy_environ(environ);
	*exit_code = ft_exit_code_holder();
	setup_interactive_signals();
	return (env);
}

/**
 * Parse input string into an AST
 * Returns the root node of the AST or NULL on error
 */
t_node	*parse_input(char *input, t_list *env)
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
