/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:21 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:21 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	option_string(const char *str)
{
	return (str && str[0] == '-' && ft_strlen(str) > 1);
}

int	valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(str[0]) && str[0] != '_'))
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

void	remove_env_var(t_env **env, const char *key)
{
	t_env	*to_del;
	t_env	*tmp;

	if (!env || !*env || !key)
		return ;
	if (ft_cmp((*env)->key, key) == 0)
	{
		to_del = *env;
		*env = to_del->next;
	}
	else
	{
		tmp = *env;
		while (tmp->next && ft_cmp(tmp->next->key, key) != 0)
			tmp = tmp->next;
		if (!tmp->next)
			return ;
		to_del = tmp->next;
		tmp->next = to_del->next;
	}
	free(to_del->key);
	free(to_del->value);
	free(to_del);
}

int	builtin_unset(t_parser *parser, t_env **env)
{
	t_lexer	*tmp;
	int		exit_status;

	tmp = parser->tokens->next;
	exit_status = 0;
	if (!tmp)
		return (0);
	if (option_string(tmp->str))
	{
		printf("unset: invalid option '%s'\n", tmp->str);
		return (2);
	}
	while (tmp && tmp->token == TOKEN_WORD)
	{
		if (!valid_identifier(tmp->str))
		{
			exit_status = 1;
			printf("unset: `%s`: not a valid identifier\n", tmp->str);
		}
		else
			remove_env_var(env, tmp->str);
		tmp = tmp->next;
	}
	return (exit_status);
}
