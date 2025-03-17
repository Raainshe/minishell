/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:49:03 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/17 17:35:31 by ksinn            ###   ########.fr       */
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

#endif
