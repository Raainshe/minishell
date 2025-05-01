/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:45:09 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 15:01:28 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Initialize the shell environment
 * Returns the environment list and sets the exit_code pointer
 */
t_list	*init_shell_environment(int **exit_code)
{
	t_list	*env;

	env = copy_environ(environ);
	*exit_code = ft_exit_code_holder();
	setup_interactive_signals();
	return (env);
}

/**
 * Read user input from terminal or pipe
 * Returns the input string or NULL if EOF was reached
 */
char	*read_input(char *pwd, bool *is_from_pipe)
{
	char	*input;
	char	*line;

	getcwd(pwd, PATH_MAX);
	ft_strlcat(pwd, "> ", PATH_MAX + 3);
	*is_from_pipe = !isatty(fileno(stdin));
	if (!(*is_from_pipe))
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
 * Process input: tokenize, parse and execute
 * Returns 0 on success, 1 on failure
 */
int	process_input(char *input, t_list **env, int *exit_code)
{
	char	**token_strings;
	t_token	*tokens;
	t_node	*ast;

	token_strings = ft_split_tokens(input);
	if (!token_strings)
		return (1);
	free(input);
	tokens = ft_tokenize(token_strings, *env);
	if (!tokens)
		return (1);
	ast = parse_tokens(tokens);
	if (!ast)
		return (1);
	setup_noninteractive_signals();
	*exit_code = execute_node(ast, env);
	return (0);
}

/**
 * Handle signals and cleanup resources after command execution
 */
void	handle_signals_and_cleanup(int *exit_code)
{
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

/**
 * Run one iteration of the shell loop
 * Returns false if the shell should exit, true otherwise
 */
bool	shell_iteration(t_list **env, int *exit_code, char *pwd)
{
	char	*input;
	bool	is_from_pipe;

	input = read_input(pwd, &is_from_pipe);
	if (!input)
	{
		if (is_from_pipe)
			return (false);
		printf("exit\n");
		return (false);
	}
	if ((input[0] == '\0' || is_only_spaces(input)) && !is_from_pipe)
	{
		free(input);
		return (true);
	}
	if (!is_from_pipe)
		add_history(input);
	if (process_input(input, env, exit_code) == 1)
		return (true);
	handle_signals_and_cleanup(exit_code);
	return (true);
}
