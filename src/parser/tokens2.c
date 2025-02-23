/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:52:17 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:52:17 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	cat_words(t_parser *parser)
{
	t_lexer	*tmp;
	t_lexer	*to_del;

	to_del = NULL;
	if (!parser || !parser->tokens)
		return ;
	tmp = parser->tokens;
	while (tmp && tmp->next)
	{
		if (tmp->space == 0)
		{
			ft_tokens_join(&tmp, &to_del);
			if (tmp->next->space == 1)
				tmp = tmp->next->next;
			free_lexer_node(&(parser->tokens), to_del);
		}
		else
			tmp = tmp->next;
	}
}

void	update_token_words(t_tools *tools, t_lexer **current)
{
	t_lexer	*tmp;

	if (!(tools->parser) || !(tools->parser->tokens))
		return ;
	tmp = *current;
	while (tmp)
	{
		if (tmp && tmp->token == TOKEN_HEREDOC)
		{
			tmp = tmp->next->next;
			continue ;
		}
		else if (tmp && tmp->token == TOKEN_WORD)
		{
			if (tmp->quote_type == DOUBLE_QUOTES)
				update_double_quotes_case(tools, tmp);
			else if (tmp->quote_type == NO_QUOTE)
			{
				update_no_quotes_case(tools, current, &tmp);
				continue ;
			}
		}
		tmp = tmp->next;
	}
}

void	update_parser_tokens(t_tools *tools)
{
	t_parser	*tmp;

	if (!(tools->parser))
		return ;
	tmp = tools->parser;
	while (tmp)
	{
		cat_heredoc_words(tmp);// test
		update_token_words(tools, &tmp->tokens);
		cat_words(tmp);
		tmp = tmp->next;
	}
}
//test