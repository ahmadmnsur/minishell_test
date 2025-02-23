/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:15:02 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/13 18:15:02 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_env_len(t_env *env)
{
	int	len;

	len = 0;
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

/* Frees a NULL-terminated array of strings. */
void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

char	**copy_envp_to_execve(t_env *env)
{
	char	**res;
	int		env_len;
	int		i;

	env_len = get_env_len(env);
	res = (char **)malloc(sizeof(char *) * (env_len + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (env)
	{
		res[i] = ft_strjoin(ft_strdup(env->key), "=");
		res[i] = ft_strjoin(res[i], env->value);
		i++;
		env = env->next;
	}
	res[i] = NULL;
	return (res);
}

char	*my_strtok(char *str, const char *delim)
{
	static char	*next;
	char		*start;

	if (!str && (!next || *next == '\0'))
		return (NULL);
	if (str)
		next = str;
	start = next;
	while (*next && !ft_strrchr(delim, *next))
		next++;
	if (*next)
		*next++ = '\0';
	return (start);
}

char	*find_path_env(t_env *env)
{
	while (env)
	{
		if (ft_cmp(env->key, "PATH") == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
