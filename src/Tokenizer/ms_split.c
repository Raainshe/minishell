/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:00:02 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/17 13:08:54 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_handle_token_start(t_token_info *info)
{
	char	current;

	current = info->input[info->i];
	if (!info->in_token && !ft_isspace(current))
	{
		info->token_start = info->i;
		info->in_token = true;
	}
	return (info->in_token);
}

static bool	ft_handle_token_end(t_token_info *info)
{
	char	current;
	char	*token;

	current = info->input[info->i];
	if (info->in_token && ft_isspace(current) && !info->in_quote
		&& !info->in_double_quote)
	{
		token = ft_strndup(&info->input[info->token_start], info->i
				- info->token_start);
		if (!token)
		{
			return (false);
		}
		info->tokens[info->j] = token;
		info->j++;
		info->in_token = false;
	}
	return (true);
}

static bool	ft_process_final_token(t_token_info *info)
{
	char	*token;

	if (info->in_token && info->j < info->count)
	{
		token = ft_strndup(&info->input[info->token_start], info->i
				- info->token_start);
		if (!token)
		{
			return (false);
		}
		info->tokens[info->j] = token;
		info->j++;
	}
	info->tokens[info->j] = NULL;
	return (true);
}

static char	**ft_process_tokens(char *str, char **tokens, int token_count)
{
	t_token_info	info;

	if (!str || !tokens)
		return (NULL);
	ft_init_token_info(&info, str, token_count);
	info.tokens = tokens;
	while (str[info.i] && info.j < token_count)
	{
		ft_update_quote_state(str[info.i], &info.in_quote,
			&info.in_double_quote);
		ft_handle_token_start(&info);
		if (!ft_handle_token_end(&info))
			return (NULL);
		info.i++;
	}
	if (!ft_process_final_token(&info))
		return (NULL);
	return (tokens);
}

char	**ft_split_tokens(char *str)
{
	char	**tokens;
	int		token_count;

	if (!str)
		return (NULL);
	token_count = ft_count_tokens(str);
	if (token_count == 0)
		token_count = 1;
	tokens = (char **)gc_malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	gc_add_context(TOKENIZER, tokens);
	return (ft_process_tokens(str, tokens, token_count));
}
