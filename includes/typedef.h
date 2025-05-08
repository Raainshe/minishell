/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:49:03 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/08 14:23:46 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEF_H
# define TYPEDEF_H

# include <stdbool.h>

typedef struct s_token_info
{
	char			**tokens;
	char			*input;
	int				count;
	int				i;
	int				j;
	int				token_start;
	bool			in_quote;
	bool			in_double_quote;
	bool			in_token;
}					t_token_info;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HERE_DOC,
	TOKEN_APPEND,
	TOKEN_END
}					t_token_type;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	bool			was_quoted;
}					t_token;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_HERE_DOC,
	NODE_APPEND,
	NODE_NULL
}					t_node_type;

typedef struct s_node
{
	t_node_type		type;
	void			*data;
	struct s_node	*left;
	struct s_node	*right;
}					t_node;

typedef struct s_command
{
	char			**args;
	t_list			*redirects;
}					t_command;

typedef struct s_redirect
{
	char			*filename;
	bool			expand_vars;
}					t_redirect;

typedef struct s_parser_context
{
	t_token			*tokens;
	int				current_index;
	int				error;
	char			*error_msg;
	int				*exit_code;
}					t_parser_context;

typedef struct s_expansion_info
{
	int				i;
	char			quote_type;
	int				in_quotes;
	char			*result;
}					t_expansion_info;

typedef struct s_main_struct
{
	char			*input;
	t_node			*ast;
	int				*exit_code;
	t_list			*env;
	bool			is_from_pipe;
}					t_main_struct;

typedef struct s_execute_pipe
{
	int				pipefd[2];
	int				status;
	pid_t			pid1;
	pid_t			pid2;
	void			*old_sigint_handler;
	void			*old_sigquit_handler;
}					t_execute_pipe;

typedef struct s_parser_redirect
{
	t_token			current;
	t_token			next_tok;
	t_node_type		redirect_type;
	char			*filename;
	t_node			*redirect_node;
	t_redirect		*redirect_data;
}					t_parser_redirect;

typedef struct s_print_export
{
	int				len;
	int				variable_len;
	int				equals_pos;
	t_list			*current;
	char			*content;
}					t_print_export;

#endif
