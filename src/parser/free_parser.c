/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:51:29 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:51:29 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	delete_parser_node(t_parser **head, t_parser *node_to_delete)
{
	t_parser	*current;

	if (head == NULL || *head == NULL || node_to_delete == NULL)
		return ;
	if (*head == node_to_delete)
	{
		*head = (*head)->next;
		free_parser_content(node_to_delete);
		free(node_to_delete);
		node_to_delete = NULL;
		return ;
	}
	current = *head;
	while (current->next != node_to_delete)
		current = current->next;
	current->next = node_to_delete->next;
	free_parser_content(node_to_delete);
	free(node_to_delete);
	node_to_delete = NULL;
}

void	free_parser_content(t_parser *node)
{
	if (node->tokens != NULL)
		free_lexer(&node->tokens);
	if (node->redirects != NULL)
		free_lexer(&node->redirects);
	if (node->hd_delimiters != NULL)
		free_split(node->hd_delimiters);
}

void	free_parser_list(t_parser **head)
{
	t_parser	*current;
	t_parser	*next;

	if (head == NULL || *head == NULL)
		return ;
	current = *head;
	while (current != NULL)
	{
		next = current->next;
		free_parser_content(current);
		free(current);
		current = next;
	}
	*head = NULL;
}
