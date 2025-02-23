/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:51:41 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:51:41 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_redirection_token(t_tokens token)
{
	if (token == TOKEN_REDIRECT_IN)
		return (1);
	if (token == TOKEN_REDIRECT_OUT)
		return (1);
	if (token == TOKEN_HEREDOC)
		return (1);
	if (token == TOKEN_APPEND)
		return (1);
	return (0);
}

int	handle_redirect_in(t_lexer *current)
{
	if (current->next && current->next->token != TOKEN_WORD && !current->space)
	{
		current = current->next;
		while (current && current->token != TOKEN_WORD && !current->space)
			current = current->next;
		if (!current)
			return (0);
	}
	return (1);
}

int	check_invalid_token(t_lexer *current)
{
	char	*token;

	if (!current->next)
	{
		print_syntax_error("newline", 0);
		return (0);
	}
	if (current->next && current->next->token != TOKEN_WORD)
	{
		token = get_token(current->token);
		print_syntax_error(token, 0);
		free(token);
		return (0);
	}
	return (1);
}

int	check_redirections(t_lexer *lexer)
{
	t_lexer	*current;

	if (!lexer)
		return (1);
	current = lexer;
	while (current != NULL)
	{
		if (is_redirection_token(current->token))
		{
			if (current->token == TOKEN_REDIRECT_IN)
			{
				if (!handle_redirect_in(current))
					return (0);
			}
			if (!check_invalid_token(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_parser_redirections(t_parser *parser)
{
	t_parser	*current;

	if (!parser)
		return (1);
	current = parser;
	while (current != NULL)
	{
		if (!check_redirections(current->tokens))
			return (0);
		current = current->next;
	}
	return (1);
}
