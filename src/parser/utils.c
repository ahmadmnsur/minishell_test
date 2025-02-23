/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:52:24 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:52:24 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_parser	*create_new_parser_node(void)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->tokens = NULL;
	parser->builtin = NULL;
	parser->redirects = NULL;
	parser->hd_delimiters = NULL;
	parser->is_pipeline_segment = 0;
	parser->next = NULL;
	parser->prev = NULL;
	return (parser);
}

void	parser_add_back(t_parser **parser)
{
	t_parser	*new_node;
	t_parser	*last_node;

	new_node = create_new_parser_node();
	if (!new_node)
		return ;
	if (*parser == NULL)
	{
		*parser = new_node;
		return ;
	}
	last_node = *parser;
	while (last_node->next != NULL)
		last_node = last_node->next;
	last_node->next = new_node;
	new_node->prev = last_node;
}

