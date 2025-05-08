/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:18:42 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/08 13:33:25 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "minishell.h"

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

/**
 * Execute a command with proper signal handling
 * Updates the exit code directly through the pointer
 */
static void	execute_with_signal_handling(t_node *ast, t_list **env,
		int *exit_code)
{
	setup_noninteractive_signals();
	*exit_code = execute_node(ast, env);
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

int	main(void)
{
	t_main_struct	t_main;

	t_main.env = init_shell(&(t_main.exit_code));
	while (42)
	{
		t_main.input = get_input(&(t_main.is_from_pipe));
		if (!t_main.input)
		{
			printf("exit\n");
			break ;
		}
		if ((t_main.input[0] == '\0' || is_only_spaces(t_main.input))
			&& !t_main.is_from_pipe)
		{
			free(t_main.input);
			continue ;
		}
		if (!t_main.is_from_pipe)
			add_history(t_main.input);
		t_main.ast = parse_input(t_main.input, t_main.env);
		if (!t_main.ast)
			continue ;
		execute_with_signal_handling(t_main.ast, &t_main.env, t_main.exit_code);
	}
	return (free_gc(), *t_main.exit_code);
}
