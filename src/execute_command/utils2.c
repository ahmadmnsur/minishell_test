/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 20:03:02 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/16 20:03:02 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*----------------------- build_args -----------------------*/
static int	count_words(t_lexer *tokens)
{
	int		count;
	t_lexer	*tmp;

	count = 0;
	tmp = tokens;
	while (tmp && tmp->token != TOKEN_PIPE && tmp->token != TOKEN_EOF)
	{
		if (tmp->token == TOKEN_WORD)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

static int	fill_args(t_lexer *tokens, char **args, int count)
{
	t_lexer	*tmp;
	int		i;

	i = 0;
	tmp = tokens;
	while (tmp && i < count)
	{
		if (tmp->token == TOKEN_WORD)
		{
			args[i] = ft_strdup(tmp->str);
			if (!args[i])
			{
				while (i > 0)
					free(args[--i]);
				return (0);
			}
			i++;
		}
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (1);
}

char	**build_args(t_lexer *tokens)
{
	char	**args;
	int		count;

	count = count_words(tokens);
	args = malloc((count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	if (!fill_args(tokens, args, count))
	{
		free(args);
		return (NULL);
	}
	return (args);
}

/*--------------------- get_command_path --------------------*/
static char	*search_paths(char *path_env, char *cmd)
{
	char	*path;
	char	*dir;
	char	*full_path;
	size_t	len;

	path = ft_strdup(path_env);
	dir = my_strtok(path, ":");
	while (dir)
	{
		len = ft_strlen(dir) + ft_strlen(cmd) + 2;
		full_path = malloc(len);
		if (full_path)
		{
			snprintf(full_path, len, "%s/%s", dir, cmd);
			if (access(full_path, F_OK) == 0)
			{
				free(path);
				return (full_path);
			}
			free(full_path);
		}
		dir = my_strtok(NULL, ":");
	}
	free(path);
	return (NULL);
}

char	*get_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	*full_path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = find_path_env(env);
	if (!path_env)
		return (NULL);
	full_path = search_paths(path_env, cmd);
	if (full_path)
		return (full_path);
	return (NULL);
}
