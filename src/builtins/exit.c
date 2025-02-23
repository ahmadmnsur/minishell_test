/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:08 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:08 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	print_error(char **arg)
{
	printf("exit: %s: numeric argument required\n", arg[1]);
	return (2);
}

int	builtin_exit(t_parser *parser, t_env **env)
{
	char	**arg;
	int		exit_status;

	(void)env;
	arg = get_cmd(parser->tokens);
	if (!arg || !arg[0])
		exit(0);
	if (!arg[1])
		exit(0);
	if (is_numeric(arg[1]))
	{
		if (arg[2])
		{
			printf("exit: too many arguments\n");
			free_args(arg);
			return (1);
		}
		exit_status = ft_atoi(arg[1]);
	}
	else
		exit_status = print_error(arg);
	free_args(arg);
	exit(exit_status);
}
