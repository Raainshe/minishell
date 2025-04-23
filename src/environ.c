/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:36:51 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/23 14:42:03 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_increment_shlvl(t_list **env, char *shlvl_str)
{
	t_list	*tmp;
	char	*old_shlvl;
	int		shlvl;
	char	*new_shlvl;

	old_shlvl = ft_strchr(shlvl_str, '=') + 1;
	if (!old_shlvl)
		return ;
	shlvl = ft_atoi(old_shlvl) % 999;
	shlvl++;
	old_shlvl = ft_itoa(shlvl);
	if (!old_shlvl)
		return ;
	gc_add_context(ENVIRON, old_shlvl);
	new_shlvl = ft_strjoin("SHLVL=", old_shlvl);
	if (!new_shlvl)
		return ;
	gc_add_context(ENVIRON, new_shlvl);
	tmp = ft_lstnew(new_shlvl);
	gc_add_context(ENVIRON, tmp);
	ft_lstadd_back(env, tmp);
}

t_list	*copy_environ(char **environ)
{
	int		i;
	t_list	*env;
	t_list	*tmp;
	char	*env_copy;

	if (!environ || !environ[0])
		return (NULL);
	env_copy = ft_strdup(environ[0]);
	if (!env_copy)
		return (NULL);
	gc_add_context(ENVIRON, env_copy);
	env = ft_lstnew(env_copy);
	gc_add_context(ENVIRON, env);
	i = 1;
	while (environ[i])
	{
		if (strncmp(environ[i], "SHLVL=", 6) == 0)
		{
			ft_increment_shlvl(&env, environ[i]);
			i++;
			continue ;
		}
		env_copy = ft_strdup(environ[i]);
		if (!env_copy)
			return (env);
		gc_add_context(ENVIRON, env_copy);
		tmp = ft_lstnew(env_copy);
		gc_add_context(ENVIRON, tmp);
		ft_lstadd_back(&env, tmp);
		i++;
	}
	return (env);
}

/**
 * @brief Convert environment linked list to char** array for execve
 * @param env The environment linked list
 * @return A char** array with environment variables
 */
char	**convert_env_to_array(t_list *env)
{
	char	**env_array;
	int		count;
	int		i;
	t_list	*current;

	if (!env)
		return (NULL);
	count = ft_lstsize(env);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	gc_add_context(EXECUTOR, env_array);
	i = 0;
	current = env;
	while (current)
	{
		env_array[i] = (char *)current->content;
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
