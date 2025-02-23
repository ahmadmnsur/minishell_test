/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_sublist.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:05:42 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/13 20:05:42 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	fill_sub_lexer(t_lexer **sub, t_lexer **tmp, unsigned int sub_size)
{
	unsigned int	i;
	t_lexer			*new;

	i = 0;
	while (i < sub_size)
	{
		if ((*tmp)->token == TOKEN_EOF)
			break;
		new = create_new_lexer_node(
				(*tmp)->str, (*tmp)->token, (*tmp)->quote_type, (*tmp)->space);
		add_lexer_node_back(sub, new);
		i++;
		(*tmp) = (*tmp)->next;
		add_index_token(sub);
	}
}

t_lexer	*find_start_node(t_lexer *lexer, t_lexer *start_node)
{
	t_lexer	*tmp;

	tmp = lexer;
	while (tmp && tmp->i < start_node->i)
		tmp = tmp->next;
	return (tmp);
}

unsigned int	count_sub_size(t_lexer *start_node, unsigned int nb_nodes)
{
	unsigned int	sub_size;
	t_lexer			*tmp;

	sub_size = 0;
	tmp = start_node;
	while (tmp && sub_size < nb_nodes)
	{
		sub_size++;
		tmp = tmp->next;
	}
	return (sub_size);
}

void	fill_missing_strings(t_lexer *sub)
{
	t_lexer	*iter;

	iter = sub;
	while (iter)
	{
		if (!iter->str)
			iter->str = get_token(iter->token);
		iter = iter->next;
	}
}

t_lexer	*ft_sublexer(t_lexer *lexer, t_lexer *start_node, unsigned int nb_nodes)
{
	t_lexer			*sub;
	t_lexer			*tmp;
	unsigned int	sub_size;

	sub = NULL;
	if (nb_nodes == 0 || !lexer)
		return (NULL);
	tmp = find_start_node(lexer, start_node);
	if (!tmp)
		return (NULL);
	sub_size = count_sub_size(tmp, nb_nodes);
	if (sub_size == 0)
		return (NULL);
	fill_sub_lexer(&sub, &tmp, sub_size);
	fill_missing_strings(sub);
	return (sub);
}
