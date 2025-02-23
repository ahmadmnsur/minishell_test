/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_limiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:51:35 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:51:35 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_nb_of_heredocs(t_lexer *redirs)
{
	int	nb_hd;

	nb_hd = 0;
	while (redirs)
	{
		if (redirs->token == TOKEN_HEREDOC)
			nb_hd++;
		redirs = redirs->next;
	}
	return (nb_hd);
}

void	set_hd_limiters_in_node(t_parser *node)
{
	int		nb_hd;
	int		i;
	t_lexer	*redirs;
	char	**limiters;

	if (!node || !node->redirects)
		return ;
	nb_hd = get_nb_of_heredocs(node->redirects);
	limiters = malloc(sizeof(char *) * (nb_hd + 1));
	if (!limiters)
		return ;
	i = 0;
	redirs = node->redirects;
	while (redirs)
	{
		if (redirs->token == TOKEN_HEREDOC && redirs->next)
		{
			limiters[i] = ft_strdup(redirs->next->str);
			i++;
		}
		redirs = redirs->next;
	}
	limiters[i] = NULL;
	node->hd_delimiters = limiters;
}

void	set_hd_limiter_parser(t_parser *parser)
{
	t_parser	*curr;

	if (!parser)
		return ;
	curr = parser;
	while (curr)
	{
		set_hd_limiters_in_node(curr);
		curr = curr->next;
	}
}
 // test