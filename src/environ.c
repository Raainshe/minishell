/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakoni <rmakoni@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:36:51 by ksinn             #+#    #+#             */
/*   Updated: 2025/04/28 14:35:36 by rmakoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get the value of an environment variable from the environment list
 * @param name Name of the variable to get
 * @param env Environment list
 * @return Value of the variable or NULL if not found
 */
char	*ft_getenv(const char *name, t_list *env)
{
	int		name_len;
	t_list	*current;
	char	*env_entry;

	if (!name || !env)
		return (NULL);
	name_len = ft_strlen(name);
	current = env;
	while (current)
	{
		env_entry = (char *)current->content;
		if (ft_strncmp(env_entry, name, name_len) == 0
			&& env_entry[name_len] == '=')
			return (env_entry + name_len + 1);
		current = current->next;
	}
	return (NULL);
}

static char	*ft_increment_shlvl(char *shlvl_str)
{
	char	*old_shlvl;
	int		shlvl;
	char	*new_shlvl;

	old_shlvl = ft_strchr(shlvl_str, '=') + 1;
	if (!old_shlvl)
		return (NULL);
	shlvl = ft_atoi(old_shlvl) % 999;
	shlvl++;
	old_shlvl = ft_itoa(shlvl);
	if (!old_shlvl)
		return (NULL);
	new_shlvl = ft_strjoin("SHLVL=", old_shlvl);
	free(old_shlvl);
	if (!new_shlvl)
		return (NULL);
	return (new_shlvl);
}

/**
 * @brief Add an environment variable to the environment list
 * @param env The environment list to add to
 * @param env_var The environment variable string to add
 * @return Updated environment list
 */
static t_list	*add_env_var(t_list **env, char *env_var)
{
	char	*env_copy;
	t_list	*tmp;

	if (strncmp(env_var, "SHLVL=", 6) == 0)
		env_copy = ft_increment_shlvl(env_var);
	else
		env_copy = ft_strdup(env_var);
	if (!env_copy)
		return (*env);
	gc_add_context(ENVIRON, env_copy);
	tmp = ft_lstnew(env_copy);
	gc_add_context(ENVIRON, tmp);
	ft_lstadd_back(env, tmp);
	return (*env);
}

/**
 * @brief Copy the environment variables into a linked list
 * @param environ Array of environment variables
 * @return Linked list containing the environment variables
 */
t_list	*copy_environ(char **environ)
{
	int		i;
	t_list	*env;
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
		add_env_var(&env, environ[i]);
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
