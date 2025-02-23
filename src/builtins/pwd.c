/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:17 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:17 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

int	pwd_help(t_env *env_list)
{
	int		nbr;
	char	*pwd;
	int		i;

	if (!env_list)
		return (1);
	nbr = env_list->two_point;
	if (nbr < 0)
		nbr = 0;
	pwd = get_env(env_list, "PWD");
	if (!pwd)
	{
		printf("Error: PWD not found in environment variables.\n");
		return (1);
	}
	printf("%s", pwd);
	i = 0;
	while (i < nbr)
	{
		printf("/..");
		i++;
	}
	printf("\n");
	return (0);
}

int	builtin_pwd(t_parser *parser, t_env **env)
{
	char	cwd[1024];

	(void)parser;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		if (!*env)
		{
			printf("Error: Environment list is NULL.\n");
			return (1);
		}
		return (pwd_help(*env));
	}
}
