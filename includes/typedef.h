/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:49:03 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/01 11:37:02 by ksinn            ###   ########.fr       */
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

typedef struct s_process_redirection
{
	t_token			current;
	t_token			next_tok;
	t_node_type		redirect_type;
	char			*filename;
	t_node			*redirect_node;
	t_redirect		*redirect_data;
}					t_process_redirection;

typedef struct s_find_command_path
{
	char			*path_var;
	char			**paths;
	char			*full_path;
	int				i;
	char			*direct_path;
}					t_find_command_path;

#endif
