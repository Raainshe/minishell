/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:51:05 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/17 13:04:24 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	create_token(char *content)
{
	t_token	token;
	size_t	len;

	token.content = content;
	token.type = TOKEN_WORD;
	len = ft_strlen(content);
	if (len == 1)
	{
		if (*content == '|')
			token.type = TOKEN_PIPE;
		else if (*content == '<')
			token.type = TOKEN_REDIRECT_IN;
		else if (*content == '>')
			token.type = TOKEN_REDIRECT_OUT;
	}
	else if (len == 2)
	{
		if (ft_strncmp(content, "<<", 2) == 0)
			token.type = TOKEN_HERE_DOC;
		else if (ft_strncmp(content, ">>", 2) == 0)
			token.type = TOKEN_APPEND;
	}
	return (token);
}

t_token	*ft_tokenize(char **tokens)
{
	t_token	*token_arr;
	int		token_count;
	int		i;

	if (!tokens)
		return (NULL);
	token_count = 0;
	while (tokens && tokens[token_count])
		token_count++;
	token_arr = (t_token *)gc_malloc(sizeof(t_token) * (token_count + 1));
	if (!token_arr)
		return (NULL);
	gc_add_context(TOKENIZER, token_arr);
	i = 0;
	while (i < token_count && tokens[i])
	{
		token_arr[i] = create_token(tokens[i]);
		i++;
	}
	token_arr[i].content = NULL;
	token_arr[i].type = TOKEN_END;
	return (token_arr);
}
