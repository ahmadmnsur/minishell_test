/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:52:05 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:52:05 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	set_one_builtin(t_parser *curr)
{
	static const char		*builtin_names[] = {
		"echo", "pwd", "cd", "export", "unset", "env", "exit"
	};
	static int				(*builtin_funcs[])(t_parser *, t_env **) = {
		builtin_echo, builtin_pwd, builtin_cd, builtin_export,
		builtin_unset, builtin_env, builtin_exit
	};
	int						i;

	i = 0;
	while (i < 7)
	{
		if (!ft_strcmp1(curr->tokens->str, builtin_names[i]) && \
			ft_strlen(curr->tokens->str) == ft_strlen(builtin_names[i]))
		{
			curr->builtin = builtin_funcs[i];
			return ;
		}
		i++;
	}
	curr->builtin = NULL;
}

void	set_builtins(t_tools *tools)
{
	t_parser	*tmp;

	tmp = tools->parser;
	while (tmp)
	{
		if (tmp->tokens)
			set_one_builtin(tmp);
		tmp = tmp->next;
	}
}

int	ft_strcmp1(const char *s1, const char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
