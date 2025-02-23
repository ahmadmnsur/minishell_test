/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:50:59 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:50:59 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* Helper: process one iteration of expansion */
static void	process_expansion(t_exp_ctx *ctx,
	int last_ret, int expand_variables)
{
	if (expand_variables && ctx->line[ctx->j] == '$' && (ctx->line[
				ctx->j + 1] == '?' || ft_isalnum((unsigned char)ctx->line[
					ctx->j + 1]) || ctx->line[ctx->j + 1] == '_'))
	{
		ctx->j++;
		if (ctx->line[ctx->j] == '?')
		{
			expand_exit_status(ctx, last_ret);
			ctx->j++;
		}
		else
			expand_env_variable(ctx);
	}
	else
		ctx->expanded[ctx->i++] = ctx->line[ctx->j++];
}

/* Main function: expand variables in a heredoc line */
char	*expand_heredoc_line(char *line, t_env *env,
	int last_ret, int expand_variables)
{
	t_exp_ctx	ctx;

	ctx.expanded = malloc(1000);
	if (!ctx.expanded)
		return (NULL);
	ctx.line = line;
	ctx.i = 0;
	ctx.j = 0;
	ctx.env = env;
	while (ctx.line[ctx.j])
		process_expansion(&ctx, last_ret, expand_variables);
	ctx.expanded[ctx.i] = '\0';
	return (ctx.expanded);
}

static int	heredoc_setup(t_heredoc_ctx *ctx, char **delimiters, \
			t_quote_type quote_type)
{
	ctx->stdin_copy = dup(STDIN_FILENO);
	if (ctx->stdin_copy == -1)
		return (perror("minishell"), -1);
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
	g_signum = 0;
	if (!delimiters || !delimiters[0])
		return (close(ctx->stdin_copy), reset_signals(), -1);
	ctx->delimiter = ft_strtrim(delimiters[0], " \t\n");
	if (!ctx->delimiter)
	{
		perror("minishell");
		return (close(ctx->stdin_copy), reset_signals(), -1);
	}
	ctx->tmp_fd = mkstemp(ctx->template);
	if (ctx->tmp_fd == -1)
	{
		perror("minishell");
		free(ctx->delimiter);
		return (close(ctx->stdin_copy), reset_signals(), -1);
	}
	ctx->expand_variables = (quote_type == NO_QUOTE);
	return (0);
}

static char	*heredoc_cleanup(t_heredoc_ctx *ctx)
{
	dup2(ctx->stdin_copy, STDIN_FILENO);
	close(ctx->stdin_copy);
	free(ctx->delimiter);
	close(ctx->tmp_fd);
	reset_signals();
	if (g_signum == SIGINT)
	{
		unlink(ctx->template);
		return (NULL);
	}
	return (ft_strdup(ctx->template));
}

char	*handle_heredoc_case(char **delimiters, t_tools *tools, \
		t_quote_type quote_type)
{
	t_heredoc_ctx	ctx;
	char			*result;
	size_t			len;

	len = ft_strlen("/tmp/minishell_heredocXXXXXX") + 1;
	ft_strlcpy(ctx.template, "/tmp/minishell_heredocXXXXXX", len);
	if (heredoc_setup(&ctx, delimiters, quote_type) != 0)
		return (NULL);
	if (heredoc_loop(&ctx, tools) != 0)
		return (NULL);
	result = heredoc_cleanup(&ctx);
	return (result);
}
