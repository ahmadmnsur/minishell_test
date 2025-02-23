/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:30 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:30 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_cmd_len(t_lexer *lexer)
{
	int	len;

	if (!lexer)
		return (0);
	len = 0;
	while (lexer && lexer->token == TOKEN_WORD)
	{
		len++;
		lexer = lexer->next;
	}
	return (len);
}

char	**get_cmd(t_lexer *lexer)
{
	char	**cmd;
	int		len;
	int		i;

	len = get_cmd_len(lexer);
	if (len == 0 || !lexer)
		return (NULL);
	cmd = (char **)malloc(sizeof(char *) * (len + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	while (lexer && lexer->token == TOKEN_WORD)
	{
		cmd[i] = ft_strdup(lexer->str);
		lexer = lexer->next;
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}

t_env	*create_env_node(char *key, char *value, int hidden)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->hidden = hidden;
	new_node->next = NULL;
	new_node->two_point = 0;
	return (new_node);
}

int	set_env(t_env **head, const char *key, const char *value, int hidden)
{
	t_env	*current;
	t_env	*prev;
	t_env	*new_node;

	current = *head;
	while (current)
	{
		if (ft_cmp(current->key, key) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
		{
			free(current->value);
			current->value = NULL;
			if (value != NULL)
				current->value = ft_strdup(value);
			current->hidden = hidden;
			return (0);
		}
		prev = current;
		current = current->next;
	}
	new_node = create_env_node((char *)key, (char *)value, hidden);
	if (!new_node)
		return (-1);
	prev->next = new_node;
	return (0);
}

void	free_split_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
