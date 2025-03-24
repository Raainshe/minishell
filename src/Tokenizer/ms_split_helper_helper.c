/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_helper_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:33:32 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/24 16:23:51 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_handle_token_start(t_token_info *info)
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

bool	ft_handle_token_end(t_token_info *info)
{
	char	current;
	char	next;

	current = info->input[info->i];
	next = info->input[info->i + 1];
	if (!info->in_token || info->in_quote || info->in_double_quote)
		return (true);
	if (ft_isspace(current))
	{
		if (!ft_add_token(info, info->token_start, info->i))
			return (false);
		info->in_token = false;
		return (true);
	}
	if (!ft_handle_operator(info, current, next))
		return (false);
	if (current != '<' && current != '>' && !ft_handle_next_operator(info,
			next))
		return (false);
	return (true);
}

bool	ft_handle_operator(t_token_info *info, char current, char next)
{
	if (ft_handle_multi_char_op(info, current, next))
		return (true);
	if (ft_is_operator(current))
	{
		if (info->token_start < info->i)
		{
			if (!ft_add_token(info, info->token_start, info->i))
				return (false);
			info->token_start = info->i;
		}
		if (!ft_add_token(info, info->token_start, info->i + 1))
			return (false);
		info->in_token = false;
	}
	return (true);
}

bool	ft_handle_next_operator(t_token_info *info, char next)
{
	if (next && ft_is_operator(next) && !info->in_quote
		&& !info->in_double_quote)
	{
		if (info->in_token)
		{
			if (!ft_add_token(info, info->token_start, info->i + 1))
				return (false);
			info->in_token = false;
		}
	}
	return (true);
}

bool	ft_handle_multi_char_op(t_token_info *info, char current, char next)
{
	if ((current == '<' && next == '<') || (current == '>' && next == '>'))
	{
		if (info->token_start < info->i)
		{
			if (!ft_add_token(info, info->token_start, info->i))
				return (false);
			info->token_start = info->i;
		}
		if (!ft_add_token(info, info->token_start, info->i + 2))
			return (false);
		info->in_token = false;
		info->i++;
		return (true);
	}
	return (false);
}
