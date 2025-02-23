/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:00:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 16:00:51 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

void	cat_heredoc_word(t_lexer **lexer, t_lexer *current)
{
	t_lexer	*tmp;
	t_lexer	*to_del;

	to_del = NULL;
	if (!current || (current && current->space))
		return ;
	tmp = *lexer;
	while (tmp != current)
		tmp = tmp->next;
	while (tmp && tmp->next && !(tmp->next->space))
	{
		to_del = tmp->next;
		tmp->str = ft_strjoin(tmp->str, to_del->str);
		tmp->space = 1;
		tmp->quote_type = SINGLE_QUOTES;
		free_lexer_node(lexer, to_del);
	}
	if (tmp && tmp->next)
	{
		to_del = tmp->next;
		tmp->str = ft_strjoin(tmp->str, to_del->str);
		tmp->space = 1;
		tmp->quote_type = SINGLE_QUOTES;
		free_lexer_node(lexer, to_del);
	}
}

void	cat_heredoc_words(t_parser *parser)
{
	t_lexer	*tmp;

	tmp = parser->tokens;
	if (!parser || !(parser->tokens))
		return ;
	while (tmp)
	{
		if (tmp->token == TOKEN_HEREDOC)
		{
			cat_heredoc_word(&(parser->tokens), tmp->next);
			if (tmp->next && tmp->next->next)
				tmp = tmp->next->next;
			else
				break;
			continue ;
		}
		else
			tmp = tmp->next;
	}
}
