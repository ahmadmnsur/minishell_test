/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:05:34 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/13 20:05:34 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_normal_string(t_lexer **lexer, const char *str, int *i)
{
	int		start;
	int		end;
	char	*sub;
	t_lexer	*new;

	start = *i;
	end = start;
	while (str[end] && !special_char(str[end]) && !whitespacee(str[end]))
		end++;
	sub = ft_substr(str, start, end - start);
	new = create_new_lexer_node(sub, TOKEN_WORD, NO_QUOTE, 1);
	if (str[end] == '"' || str[end] == '\'')
		new->space = 0;
	add_lexer_node_back(lexer, new);
	free(sub);
	*i = end;
}

// Inside handle_pipe function in lexer.c
void    handle_pipe11(const char *s, int *i, t_lexer **lexer_list)
{
    if (s[*i] == '|')
    {
        add_lexer_node_back(lexer_list, create_new_lexer_node(ft_strdup(""), TOKEN_PIPE, NO_QUOTE, 0)); // Tokenize pipe
        (*i)++;
    }
}
void handle_pipe(t_lexer **lexer_list, int *i) { // Signature as requested by user

    // Directly create and add PIPE token - NO MORE check for s[*i] == '|'
    t_lexer *pipe_token = create_new_lexer_node(ft_strdup("|"), TOKEN_PIPE, NO_QUOTE, 0); // NONE for no quote
    if (!pipe_token) {
        // Handle memory allocation error
        fprintf(stderr, "Error: Failed to create lexer node for PIPE\n");
        return; // Or handle error appropriately
    }
    add_lexer_node_back(lexer_list, pipe_token);
    (*i)++; // Increment index - MOVE PAST the assumed '|' character
}

void	handle_in_or_heredoc(t_lexer **lexer, const char *str, int *i)
{
	if (str[*i + 1] && str[*i + 1] == '<')
		add_lexer_and_increment_i(lexer, TOKEN_HEREDOC, i, 2);
	else
		add_lexer_and_increment_i(lexer, TOKEN_REDIRECT_IN, i, 1);
}

void	handle_out_or_append(t_lexer **lexer, const char *str, int *i)
{
	if (str[*i + 1] && str[*i + 1] == '>')
		add_lexer_and_increment_i(lexer, TOKEN_APPEND, i, 2);
	else
		add_lexer_and_increment_i(lexer, TOKEN_REDIRECT_OUT, i, 1);
}


void handle_quotes(t_lexer **lexer, const char *str, int *i, char quote)
{
    char            *sub;
    t_lexer         *new;
    t_quote_type    type;
    int             len;

    type = get_quote_type(quote);
    len = get_quote_length(str, *i, quote);
    sub = ft_substr(str, *i + 1, len - 1);
    
    // If it's a double-quoted string, remove escape backslashes.
    if (type == DOUBLE_QUOTES)
    {
        char *tmp = remove_escaped_quotes(sub);
        free(sub);
        sub = tmp;
    }

    new = create_new_lexer_node(sub, TOKEN_WORD, type, 1);
    if (str[*i + len + 1] && !whitespacee(str[*i + len + 1]) && str[*i + len + 1] != '<'
        && str[*i + len + 1] != '>' && str[*i + len + 1] != '|')
        new->space = 0;
    add_lexer_node_back(lexer, new);
    free(sub);
    *i += len + 1;
}

// 3. Add a new handler for semicolons
void handle_semicolon(t_lexer **lexer, int *i)
{
    t_lexer *new;

    new = create_new_lexer_node(ft_strdup(";"), TOKEN_SEMICOLON, -1, 1); // strdup here
    add_lexer_node_back(lexer, new);
    (*i)++;
}
