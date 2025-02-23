/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe21.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:31:45 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/16 22:31:45 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	process_heredoc_fd(t_tools *tools, t_lexer *current)
{
	char	*filename;

	rl_clear_history();
	filename = handle_heredoc_case(tools->parser->hd_delimiters,
			tools, current->quote_type);
	if (!filename)
		return (write(2, "minishell: error processing heredoc\n", 36), -1);
	return (open(filename, O_RDONLY));
}

int	open_fd_for_redirection(t_tools *tools, t_lexer *current)
{
	char	*filename;

	if (!current->next || current->next->token != TOKEN_WORD)
		return (write(2, "syntax error near unexpected token\n", 35), -1);
	filename = current->next->str;
	if (!filename)
		return (write(2, "error processing redirection\n", 30), -1);
	if (current->token == TOKEN_HEREDOC)
		return (process_heredoc_fd(tools, current));
	if (current->token == TOKEN_REDIRECT_IN)
		return (open(filename, O_RDONLY));
	if (current->token == TOKEN_REDIRECT_OUT)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (current->token == TOKEN_APPEND)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (write(2, "syntax error near unexpected token\n", 35), -1);
}

int	dup_fd_for_token(int fd, int token)
{
	if (token == TOKEN_REDIRECT_IN || token == TOKEN_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
			return (-1);
		}
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
			return (-1);
		}
	}
	return (0);
}

int	process_redirections_child_for_no_pipe(t_tools *tools, t_lexer *redirects)
{
	t_lexer	*current;
	int		fd;

	current = redirects;
	while (current)
	{
		fd = open_fd_for_redirection(tools, current);
		if (fd == -1)
			return (-1);
		if (dup_fd_for_token(fd, current->token) == -1)
		{
			close(fd);
			return (-1);
		}
		close(fd);
		current = current->next->next;
	}
	return (0);
}

int	execute_builtin_no_pipe(t_tools *tools, t_parser *parser)
{
	int	exit_status;
	int	save_stdin;
	int	save_stdout;
	int	save_stderr;

	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	save_stderr = dup(STDERR_FILENO);
	if (parser->redirects && process_redirections_child_for_no_pipe
		(tools, parser->redirects) != 0)
		exit_status = 1;
	else
		exit_status = parser->builtin(parser, &tools->env);
	dup2(save_stdin, STDIN_FILENO);
	close(save_stdin);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdout);
	dup2(save_stderr, STDERR_FILENO);
	close(save_stderr);
	return (exit_status);
}
