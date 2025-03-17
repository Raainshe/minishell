/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:53:15 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/17 13:03:41 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

void	ft_update_quote_state(char c, bool *in_quote, bool *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_quote = !(*in_quote);
	else if (c == '\"' && !(*in_quote))
		*in_double_quote = !(*in_double_quote);
}

static void	ft_init_count_vars(int *count, int *i, bool *in_quote,
	bool *in_double_quote, bool *in_token)
{
	*count = 0;
	*i = 0;
	*in_quote = false;
	*in_double_quote = false;
	*in_token = false;
}

static void	ft_process_token_state(char c, bool *in_token, int *count,
	bool in_quote, bool in_double_quote)
{
	if (!(*in_token) && !ft_isspace(c))
	{
		*in_token = true;
		(*count)++;
	}
	else if (*in_token && ft_isspace(c) && !in_quote && !in_double_quote)
		*in_token = false;
}

int	ft_count_tokens(char *str)
{
	int		count;
	int		i;
	bool	in_quote;
	bool	in_double_quote;
	bool	in_token;

	if (!str)
		return (0);
	ft_init_count_vars(&count, &i, &in_quote, &in_double_quote, &in_token);
	while (str[i])
	{
		ft_update_quote_state(str[i], &in_quote, &in_double_quote);
		ft_process_token_state(str[i], &in_token, &count,
			in_quote, in_double_quote);
		i++;
	}
	return (count);
}

/*
** This function should only be used for manual cleanup when needed
** In most cases, use gc_free_context(TOKENIZER) instead
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

void	ft_init_token_info(t_token_info *info, char *str, int token_count)
{
	if (!info || !str)
		return ;
	info->input = str;
	info->count = token_count;
	info->i = 0;
	info->j = 0;
	info->in_quote = false;
	info->in_double_quote = false;
	info->in_token = false;
}
