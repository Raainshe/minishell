/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:49:03 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/18 14:49:24 by ksinn            ###   ########.fr       */
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
}					t_token;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_HERE_DOC,
	NODE_APPEND,
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
}					t_redirect;

typedef struct s_parser_context
{
	t_token			*tokens;
	int				current_index;
	int				error;
	char			*error_msg;
}					t_parser_context;

#endif
