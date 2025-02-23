/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:01 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:01 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	is_valid_n(const char *arg)
{
	int	i;

	if (arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i] != '\0')
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	handle_newline_option(char **arg, int *new_line)
{
	int	i;

	i = 0;
	while (arg[i] && is_valid_n(arg[i]))
	{
		*new_line = 0;
		i++;
	}
	return (i);
}

int	builtin_echo(t_parser *parser, t_env **env)
{
	char	**arg;
	int		new_line;
	int		i;

	(void)env;
	arg = get_cmd(parser->tokens->next);
	if (!arg || !arg[0])
	{
		printf("\n");
		return (0);
	}
	new_line = 1;
	i = handle_newline_option(arg, &new_line);
	while (arg[i])
	{
		printf("%s", arg[i]);
		if (arg[i + 1])
			printf(" ");
		i++;
	}
	if (new_line)
		printf("\n");
	return (0);
}
