/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:45:09 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/03 00:17:07 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialize the shell environment and setup signals
 *
 * Copies the environment variables into a linked list, sets up the
 * exit code holder, and configures interactive signal handling.
 *
 * @param exit_code Pointer to store the address of exit code holder
 * @return Linked list containing the shell environment variables
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
 * @brief Read user input from terminal or pipe
 *
 * Formats and displays the prompt with current working directory when
 * reading from terminal. Handles input from both terminal and pipes,
 * adjusting behavior accordingly.
 *
 * @param pwd Buffer to store the prompt string
 * @param is_from_pipe Pointer to store whether input is from pipe
 * @return User input string or NULL if EOF was reached
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
 * @brief Process user input through the shell's execution pipeline
 *
 * Takes raw user input, tokenizes it, creates an abstract syntax tree,
 * and executes the commands. Handles signal setup for command execution
 * and tracks the exit status of commands.
 *
 * @param input The raw input string from the user
 * @param env Pointer to the environment variables list
 * @param exit_code Pointer to store the command exit status
 * @return 0 on successful processing, 1 on failure
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
 * @brief Handle signals and cleanup resources after command execution
 *
 * Updates the exit code based on received signals, resets the terminal
 * state if needed, and frees allocated memory for tokenizer, AST, and
 * executor contexts.
 *
 * @param exit_code Pointer to store the exit status
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
 * @brief Run one iteration of the shell loop
 *
 * Reads and processes user input, adding non-empty commands to history.
 * Handles signals, terminal states, and resource cleanup. Determines
 * whether the shell should exit or continue based on input.
 *
 * @param env Pointer to the environment variables list
 * @param exit_code Pointer to store the command exit status
 * @param pwd Buffer to store the prompt string
 * @return false if the shell should exit, true otherwise
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
