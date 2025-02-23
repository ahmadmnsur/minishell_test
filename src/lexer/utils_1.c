/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_quote_and_string.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:00:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/04 15:56:44 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_quote_type	get_quote_type(char quote)
{
	if (quote == '\'')
		return (SINGLE_QUOTES);
	return (DOUBLE_QUOTES);
}

// int	get_quote_length(const char *str, int i, char quote)
// {
// 	int	len;

// 	len = 1;
// 	while (str[i + len] && str[i + len] != quote)
// 		len++;
// 	if (str[i + len] != quote)
// 		return ((int)ft_strlen(str) - i - 1);
// 	return (len);
// }

int	get_quote_length(const char *str, int i, char quote)
{
    int	len;

	len = 1;
    while (str[i + len])
    {
        if (quote == '"' && str[i + len] == '\\' && str[i + len + 1] == '"')
        {
            // Skip both the backslash and the escaped quote.
            len += 2;
            continue;
        }
        if (str[i + len] == quote)
            return (len);
        len++;
    }
    return (len);
}

void handle_special_char(t_lexer **lexer, const char *str, int *i)
{
    if (str[*i] == '<')
        handle_in_or_heredoc(lexer, str, i);
    else if (str[*i] == '\'' || str[*i] == '\"')
        handle_quotes(lexer, str, i, str[*i]);
    else if (str[*i] == '|')
        handle_pipe(lexer, i);
    else if (str[*i] == '>')
        handle_out_or_append(lexer, str, i);
    else if (str[*i] == ';')
        handle_semicolon(lexer, i);
}

void	process_string(t_lexer **lexer, const char *str, int *i)
{
	if (whitespacee(str[*i]))
		skip_spaces(str, i);
	else if (special_char(str[*i]))
		handle_special_char(lexer, str, i);
	else
		handle_normal_string(lexer, str, i);
}

void	add_lexer_and_increment_i(t_lexer **lexer, int token,
		int *i, int increment)
{
	t_lexer	*new;

	new = create_new_lexer_node(get_token(token), token, -1, 1);
	*i += increment;
	add_lexer_node_back(lexer, new);
}

int	get_nb_of_pipes(t_lexer *lexer)
{
	int		nb_pipes;
	t_lexer	*tmp;

	nb_pipes = 0;
	if (!lexer || lexer->token == TOKEN_EOF)
		return (0);
	tmp = lexer;
	while (tmp)
	{
		if (tmp->token == TOKEN_PIPE)
			nb_pipes++;
		tmp = tmp->next;
	}
	return (nb_pipes);
}
