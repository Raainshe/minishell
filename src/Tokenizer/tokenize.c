/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:51:05 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/29 14:34:15 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Manually frees an array of tokens
 * @param tokens The array of tokens to free
 * @param j The number of tokens to free
 * @return None
 *
 * This function should only be used for manual cleanup when needed.
 * In most cases, use gc_free_context(TOKENIZER) instead.
 */
void	ft_free_tokens(char **tokens, int j)
{
	if (!tokens)
		return ;
	while (j > 0 && tokens[j - 1])
	{
		j--;
		if (tokens[j])
			free(tokens[j]);
	}
	free(tokens);
}

/**
 * @brief Checks if a string contains quotes
 * @param str The string to check
 * @return true if the string contains single or double quotes, false otherwise
 */
bool	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (true);
		i++;
	}
	return (false);
}

static void	initialise_token_variables(bool *was_quoted, t_token *token,
		char **content, t_list *env)
{
	*was_quoted = has_quotes(*content);
	token->was_quoted = *was_quoted;
	*content = expand(*content, env);
	token->content = *content;
	token->type = TOKEN_WORD;
}

/**
 * @brief Creates a token structure from a string content
 * @param content The string content to create a token from
 * @param env The environment variables list
 * @return A t_token structure with the appropriate token type
 */
static t_token	create_token(char *content, t_list *env)
{
	t_token	token;
	size_t	len;
	bool	was_quoted;

	initialise_token_variables(&was_quoted, &token, &content, env);
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
		if (ft_strncmp("<<", content, 2) == 0)
			token.type = TOKEN_HERE_DOC;
		else if (ft_strncmp(">>", content, 2) == 0)
			token.type = TOKEN_APPEND;
	}
	return (token);
}

/**
 * @brief Converts an array of string tokens to an array of t_token structures
 * @param tokens The array of string tokens to convert
 * @return A pointer to the array of t_token structures, NULL if error
 */
t_token	*ft_tokenize(char **tokens, t_list *env)
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
		token_arr[i] = create_token(tokens[i], env);
		i++;
	}
	token_arr[i].content = NULL;
	token_arr[i].type = TOKEN_END;
	return (token_arr);
}
