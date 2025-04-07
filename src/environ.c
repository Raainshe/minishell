/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:36:51 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/07 17:28:52 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*copy_environ(char **environ)
{
	int		i;
	t_list	*env;

	if (!environ || !environ[0])
		return (NULL);
	env = ft_lstnew(environ[0]);
	gc_add_context(ENVIRON, env);
	i = 1;
	while (environ[i])
	{
		ft_lstadd_back(&env, ft_lstnew(environ[i]));
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
