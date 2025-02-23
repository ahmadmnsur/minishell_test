/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:52:11 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:52:11 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// static unsigned int	count_lexer_nodes(t_lexer *lexer)
// {
// 	t_lexer			*tmp_lexer;
// 	unsigned int	nb_nodes;

// 	nb_nodes = 0;
// 	tmp_lexer = lexer;
// 	while (tmp_lexer)
// 	{
// 		nb_nodes++;
// 		tmp_lexer = tmp_lexer->next;
// 	}
// 	return (nb_nodes);
// }


// static void	assign_tokens_to_parser(t_parser **parser,
// 	t_lexer *lexer, unsigned int nb_nodes)
// {
// t_lexer		*tmp_lexer;
// t_lexer		*start_node;
// t_parser	*tmp_parser;

// tmp_lexer = lexer;
// tmp_parser = *parser;
// start_node = tmp_lexer;
// while (tmp_parser && tmp_lexer)
// {
// if (start_node && start_node->token == TOKEN_EOF)
// 	break;
// while (tmp_lexer && tmp_lexer->token != TOKEN_PIPE && tmp_lexer->token != TOKEN_SEMICOLON)
// 	tmp_lexer = tmp_lexer->next;
// if (tmp_lexer)
// {
// 	// Create a sublexer for tokens before the delimiter.
// 	tmp_parser->tokens = ft_sublexer(lexer, start_node,
// 			tmp_lexer->i - start_node->i);
// 	// Record which delimiter ended this command.
// 	if (tmp_lexer->token == TOKEN_PIPE)
// 		tmp_parser->delim = TOKEN_PIPE;
// 	else if (tmp_lexer->token == TOKEN_SEMICOLON)
// 		tmp_parser->delim = TOKEN_SEMICOLON;
// 	// Advance to the token after the delimiter.
// 	start_node = tmp_lexer->next;
// 	//tmp_lexer = tmp_lexer->next;
// }
// else
// {
// 	tmp_parser->tokens = ft_sublexer(lexer, start_node,
// 			nb_nodes - start_node->i);
// 	tmp_parser->delim = NO_DELIM;
// }
// tmp_parser = tmp_parser->next;
// }
// }


//# Replace set_tokens_lexer with empty function
void        set_tokens_lexer(t_parser **parser, t_lexer *lexer)
{
    (void)parser;
    (void)lexer;
}

char	*get_token(t_tokens token)
{
	if (token == TOKEN_REDIRECT_IN)
		return (ft_strdup("<"));
	if (token == TOKEN_REDIRECT_OUT)
		return (ft_strdup(">"));
	if (token == TOKEN_HEREDOC)
		return (ft_strdup("<<"));
	if (token == TOKEN_APPEND)
		return (ft_strdup(">>"));
	if (token == TOKEN_SEMICOLON)
		return (ft_strdup(";"));
	if (token == TOKEN_PIPE)
		return (ft_strdup("|"));
	return (ft_strdup(""));
}

void	ft_tokens_join(t_lexer **tmp, t_lexer **to_del)
{
	(*to_del) = (*tmp)->next;
	if (ft_strlen((*tmp)->str) == 1 && (*tmp)->str[0] == '$'
		&& (*tmp)->quote_type == NO_QUOTE)
	{
		free((*tmp)->str);
		(*tmp)->str = ft_strdup((*tmp)->next->str);
	}
	else{
		(*tmp)->str = ft_strjoin((*tmp)->str, (*tmp)->next->str);
	}
}

void	free_strs_nodes(char **s1, char **s2)
{
	free(*s1);
	*s1 = ft_strdup(*s2);
	free(*s2);
	*s2 = NULL;
}

void	delete_empty_node(t_lexer **tokens, t_lexer **tmp, char **str)
{
	t_lexer	*to_del;

	to_del = *tmp;
	(*tmp) = (*tmp)->next;
	free_lexer_node(tokens, to_del);
	free(*str);
}
//testt