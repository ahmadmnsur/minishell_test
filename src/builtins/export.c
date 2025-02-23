/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:13 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:13 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**export_split(const char *str, const char *delimiter)
{
	char	**result;
	size_t	delim_len;
	char	*delim_pos;

	result = malloc(5 * sizeof(char *));
	if (!result)
		return (NULL);
	delim_len = ft_strlen(delimiter);
	delim_pos = ft_strstr(str, delimiter);
	if (!delim_pos)
	{
		result[0] = NULL;
		result[1] = ft_strdup(str);
		result[2] = NULL;
		result[3] = NULL;
		result[4] = NULL;
		return (result);
	}
	result[0] = NULL;
	result[1] = ft_strndupp(str, delim_pos - str);
	result[2] = ft_strndupp(delimiter, delim_len);
	result[3] = ft_strdup(delim_pos + delim_len);
	result[4] = NULL;
	return (result);
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	handle_invalid_export(char **argcomment)
{
	printf("export: `%s`: not a valid identifier\n", argcomment[1]);
	free_split_array(argcomment);
	return (1);
}

int	check_existing_key(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_cmp(current->key, key) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

int	builtin_export(t_parser *parser, t_env **env)
{
	char	**arg;
	char	**argcomment;
	int		key_exists;

	arg = get_cmd(parser->tokens);
	if (!arg[1])
		return (print_full_export_list(*env), 0);
	argcomment = export_split(arg[1], "=");
	if (!argcomment || !argcomment[1])
		return (printf("export: Invalid input\n"),
			free_split_array(argcomment), 1);
	if (!is_valid_identifier(argcomment[1]))
		return (handle_invalid_export(argcomment));
	key_exists = check_existing_key(*env, argcomment[1]);
	if (!argcomment[2] && !key_exists)
		add_env_var(env, argcomment[1], NULL);
	else if (argcomment[2] && !argcomment[3])
		update_env_var(env, argcomment[1], "");
	else if (argcomment[2] && argcomment[3])
		update_env_var(env, argcomment[1], argcomment[3]);
	free_split_array(argcomment);
	return (0);
}
