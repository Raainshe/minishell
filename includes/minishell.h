/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:18:57 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/12 14:57:14 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_token_info
{
	char	**tokens;
	char	*input;
	int		count;
	int		i;
	int		j;
	int		token_start;
	bool	in_quote;
	bool	in_double_quote;
	bool	in_token;
}			t_token_info;

// ms_split_helper.c
int			ft_isspace(char c);
void		ft_update_quote_state(char c, bool *in_quote,
				bool *in_double_quote);
int			ft_count_tokens(char *str);
void		ft_free_tokens(char **tokens, int j);
void		ft_init_token_info(t_token_info *info, char *str, int token_count);
// ms_split.c
char		**ft_split_tokens(char *str);
// ft_strndup.c
char		*ft_strndup(const char *s, size_t n);

#endif
