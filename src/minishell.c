/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:18:42 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/12 17:20:46 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "minishell.h"

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
 * Set up heredocs and handle preprocessing signals
 * Returns preprocessing result code
 */
static int	setup_heredocs_with_signals(t_node *ast, t_list **env)
{
	int	preprocess_result;

	init_heredoc_fds(ast);
	setup_noninteractive_signals();
	preprocess_result = preprocess_heredocs(ast, env);
	return (preprocess_result);
}

/**
 * Execute command and handle any signals that occur during execution
 * Returns the appropriate exit code based on execution result
 */
static int	handle_command_execution(t_node *ast, t_list **env)
{
	int	exit_code;

	exit_code = execute_node(ast, env);
	if (g_signal_received)
	{
		exit_code = get_signal_status();
		reset_term_after_signal();
	}
	else
		setup_interactive_signals();
	return (exit_code);
}

/**
 * Execute a command with proper signal handling
 * Updates the exit code directly through the pointer
 */
static void	execute_with_signal_handling(t_node *ast, t_list **env,
		int *exit_code)
{
	int	preprocess_result;

	preprocess_result = setup_heredocs_with_signals(ast, env);
	if (preprocess_result < 0)
	{
		if (preprocess_result == -130)
		{
			*exit_code = 130;
			g_signal_received = SIGINT;
		}
		else
			*exit_code = 1;
		reset_term_after_signal();
		setup_interactive_signals();
		cleanup_execution_resources(ast);
		return ;
	}
	*exit_code = handle_command_execution(ast, env);
	cleanup_execution_resources(ast);
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
