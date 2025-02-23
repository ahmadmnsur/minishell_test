/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:51:17 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:51:17 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_lexer	*create_new_lexer_node(char *str, t_tokens token,
		t_quote_type quote_type, int space)
{
	t_lexer	*new_node;

	new_node = (t_lexer *)calloc(1, sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	if (str)
		new_node->str = ft_strdup(str);
	else
		new_node->str = NULL;
	new_node->token = token;
	new_node->quote_type = quote_type;
	new_node->space = space;
	new_node->i = -1;
	new_node->next = NULL;
	return (new_node);
}

void	add_lexer_node_back(t_lexer **head, t_lexer *new_node)
{
	t_lexer	*temp;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
}

void	add_index_token(t_lexer **lexer)
{
	t_lexer	*tmp;
	int		i;

	i = 0;
	tmp = *lexer;
	if (!*lexer)
		return ;
	while (tmp)
	{
		tmp->i = i;
		i++;
		tmp = tmp->next;
	}
}

void	free_lexer(t_lexer **lexer)
{
	t_lexer	*tmp;
	t_lexer	*next_node;

	if (!lexer || !*lexer)
		return ;
	tmp = *lexer;
	while (tmp != NULL)
	{
		next_node = tmp->next;
		if (tmp->str != NULL)
			free(tmp->str);
		free(tmp);
		tmp = next_node;
	}
	*lexer = NULL;
}

int	count_pipes(t_lexer *node)
{
	int		pipe_count;
	t_lexer	*current;

	pipe_count = 0;
	if (node == NULL || node->token == TOKEN_EOF)
		return (0);
	current = node;
	while (current != NULL)
	{
		if (current->token == TOKEN_PIPE)
			pipe_count++;
		current = current->next;
	}
	return (pipe_count);
}
