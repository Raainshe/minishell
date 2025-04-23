/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:18:42 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/23 16:37:16 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "minishell.h"

// static void	print_ast(t_node *node, int depth)
// {
// 	int			i;
// 	t_command	*cmd;
// 	int			j;

// 	if (!node)
// 		return ;
// 	i = 0;
// 	while (i < depth)
// 	{
// 		printf("  ");
// 		i++;
// 	}
// 	if (node->type == NODE_COMMAND)
// 	{
// 		cmd = (t_command *)node->data;
// 		printf("COMMAND: %s\n", cmd->args[0]);
// 		i = 1;
// 		while (cmd->args[i])
// 		{
// 			j = 0;
// 			while (j < depth + 1)
// 			{
// 				printf("  ");
// 				j++;
// 			}
// 			printf("ARG: %s\n", cmd->args[i]);
// 			i++;
// 		}
// 	}
// 	else if (node->type == NODE_PIPE)
// 		printf("PIPE\n");
// 	else if (node->type == NODE_REDIRECT_IN)
// 		printf("REDIRECT_IN: %s\n", ((t_redirect *)node->data)->filename);
// 	else if (node->type == NODE_REDIRECT_OUT)
// 		printf("REDIRECT_OUT: %s\n", ((t_redirect *)node->data)->filename);
// 	else if (node->type == NODE_HERE_DOC)
// 		printf("HERE_DOC: %s\n", ((t_redirect *)node->data)->filename);
// 	else if (node->type == NODE_APPEND)
// 		printf("APPEND: %s\n", ((t_redirect *)node->data)->filename);
// 	print_ast(node->left, depth + 1);
// 	print_ast(node->right, depth + 1);
// }

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

// TODO: only isspace should prompt new line
int	main(void)
{
	char	*input;
	t_token	*tokens;
	char	**token_strings;
	t_node	*ast;
	int		*exit_code;
	char	pwd[PATH_MAX + 3];
	char	*line;
	t_list	*env;

	print_banner();
	env = copy_environ(environ);
	exit_code = ft_exit_code_holder();
	setup_interactive_signals();
	while (42)
	{
		getcwd(pwd, PATH_MAX);
		ft_strlcat(pwd, "> ", PATH_MAX + 3);
		// input = readline((const char *)pwd);
		if (isatty(fileno(stdin)))
			input = readline(pwd);
		else
		{
			line = get_next_line(fileno(stdin));
			input = ft_strtrim(line, "\n");
			free(line);
		}
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (input[0] == '\0' || is_only_spaces(input))
			continue ;
		add_history(input);
		token_strings = ft_split_tokens(input);
		if (!token_strings)
		{
			printf("ft_split_tokens error\n");
			free(input);
			continue ;
		}
		free(input);
		tokens = ft_tokenize(token_strings, env);
		if (!tokens)
		{
			printf("ft_tokenize error\n");
			continue ;
		}
		ast = parse_tokens(tokens);
		if (!ast)
		{
			printf("parse_tokens error\n");
			continue ;
		}
		// print_ast(ast, 0); // Uncomment for debugging
		setup_noninteractive_signals();
		*exit_code = execute_node(ast, &env);
		if (g_signal_received)
			*exit_code = get_signal_status();
		setup_interactive_signals();
		gc_free_context(TOKENIZER);
		gc_free_context(AST);
		gc_free_context(EXECUTOR);
	}
	free_gc();
	return (*exit_code);
}
