/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:50:13 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:50:13 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_env(t_parser *parser, t_env **env)
{
	t_env	*tmp;

	(void)parser;
	if (!*env)
	{
		fprintf(stderr, "env: No environment variables to display\n");
		return (0);
	}
	tmp = *env;
	while (tmp != NULL)
	{
		if (tmp->hidden == 0)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
