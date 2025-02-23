/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:06:31 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/18 11:06:31 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_sigint(t_heredoc_ctx *ctx, char *line)
{
	free(line);
	close(ctx->tmp_fd);
	unlink(ctx->template);
	free(ctx->delimiter);
	dup2(ctx->stdin_copy, STDIN_FILENO);
	close(ctx->stdin_copy);
	reset_signals();
	return (-1);
}

int	process_heredoc_line(t_heredoc_ctx *ctx, t_tools *tools, char *line)
{
	char	*expanded_line;

	if (ft_cmp(line, ctx->delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (!ctx->expand_variables)
	{
		expanded_line = expand_heredoc_line(line, tools->env,
				tools->last_exit_status, 1);
		ft_putendl_fd(expanded_line, ctx->tmp_fd);
		free(expanded_line);
	}
	else
		ft_putendl_fd(line, ctx->tmp_fd);
	free(line);
	return (0);
}

int	heredoc_loop(t_heredoc_ctx *ctx, t_tools *tools)
{
	char	*line;
	int		ret;

	while (1)
	{
		line = readline("> ");
		if (g_signum == SIGINT)
			return (handle_sigint(ctx, line));
		if (!line)
		{
			print_heredoc_error(ctx->delimiter);
			break ;
		}
		ret = process_heredoc_line(ctx, tools, line);
		if (ret)
			break ;
	}
	return (0);
}

void	expand_exit_status(t_exp_ctx *ctx, int last_ret)
{
	char	ret_str[12];
	size_t	k;

	snprintf(ret_str, sizeof(ret_str), "%d", last_ret);
	k = 0;
	while (ret_str[k])
		ctx->expanded[ctx->i++] = ret_str[k++];
}

void	expand_env_variable(t_exp_ctx *ctx)
{
	size_t	key_start;
	char	*key;
	t_env	*env_var;
	size_t	k;

	key_start = ctx->j;
	while (ft_isalnum((unsigned char)ctx->line[
				ctx->j]) || ctx->line[ctx->j] == '_')
		ctx->j++;
	key = ft_strndupp(ctx->line + key_start, ctx->j - key_start);
	if (!key)
		return ;
	env_var = get_env_var(ctx->env, key);
	free(key);
	if (env_var && env_var->value)
	{
		k = 0;
		while (env_var->value[k])
			ctx->expanded[ctx->i++] = env_var->value[k++];
	}
}
