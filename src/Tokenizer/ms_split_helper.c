/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:53:15 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/17 14:45:54 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_update_quote_state(char c, bool *in_quote, bool *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_quote = !(*in_quote);
	else if (c == '\"' && !(*in_quote))
		*in_double_quote = !(*in_double_quote);
}

/* Check if we have a multi-character operator (<< or >>) at position i */
static bool	ft_is_multi_char_op(char *str, int i)
{
	if (!str || !str[i] || !str[i + 1])
		return (false);
	return ((str[i] == '<' && str[i + 1] == '<') || (str[i] == '>' && str[i
				+ 1] == '>'));
}

static void	ft_process_token_state(t_token_info *info, int i, int *count)
{
	char	current;

	current = info->input[i];
	if (!(info->in_token) && !ft_isspace(current))
	{
		info->in_token = true;
		(*count)++;
	}
	else if (info->in_token && (ft_isspace(current) || (ft_is_operator(current)
				&& !info->in_quote && !info->in_double_quote)))
	{
		info->in_token = false;
	}
	if (ft_is_operator(current) && !info->in_quote && !info->in_double_quote
		&& !(info->in_token))
	{
		(*count)++;
		info->in_token = true;
		if (ft_is_multi_char_op(info->input, i))
			info->in_token = false;
	}
}

int	ft_count_tokens(char *str)
{
	int				count;
	int				i;
	t_token_info	info;

	if (!str)
		return (0);
	count = 0;
	i = 0;
	ft_init_token_info(&info, str, 0);
	while (str[i])
	{
		ft_update_quote_state(str[i], &info.in_quote, &info.in_double_quote);
		ft_process_token_state(&info, i, &count);
		if (!info.in_quote && !info.in_double_quote && ft_is_multi_char_op(str,
				i))
			i++;
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
