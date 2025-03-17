/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:53:15 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/17 13:17:08 by ksinn            ###   ########.fr       */
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

static void	ft_process_token_state(char c, bool *in_token, int *count,
		t_quote_state *q_state)
{
	if (!(*in_token) && !ft_isspace(c))
	{
		*in_token = true;
		(*count)++;
	}
	else if (*in_token && ft_isspace(c) && !q_state->in_quote
		&& !q_state->in_double_quote)
		*in_token = false;
}

int	ft_count_tokens(char *str)
{
	int				count;
	int				i;
	bool			in_token;
	t_quote_state	q_state;

	if (!str)
		return (0);
	count = 0;
	i = 0;
	in_token = false;
	q_state.in_quote = false;
	q_state.in_double_quote = false;
	while (str[i])
	{
		ft_update_quote_state(str[i], &q_state.in_quote,
			&q_state.in_double_quote);
		ft_process_token_state(str[i], &in_token, &count, &q_state);
		i++;
	}
	return (count);
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
