/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:00:02 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/17 14:39:23 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_add_token(t_token_info *info, int start, int end)
{
	char	*token;

	token = ft_strndup(&info->input[start], end - start);
	if (!token)
		return (false);
	info->tokens[info->j] = token;
	info->j++;
	return (true);
}

/* Handle case where we have a token after the last processed character */
static bool	ft_handle_remaining_tokens(char *str, t_token_info *info)
{
	int	remaining_start;

	remaining_start = info->i;
	while (str[remaining_start] && ft_isspace(str[remaining_start]))
		remaining_start++;
	if (str[remaining_start])
	{
		info->i = ft_strlen(str);
		return (ft_add_token(info, remaining_start, info->i));
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
			return (false);
		info->tokens[info->j] = token;
		info->j++;
	}
	if (info->input[info->i] && info->j < info->count)
	{
		if (!ft_handle_remaining_tokens(info->input, info))
			return (false);
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
	token_count += 1;
	tokens = (char **)gc_malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	gc_add_context(TOKENIZER, tokens);
	return (ft_process_tokens(str, tokens, token_count));
}
