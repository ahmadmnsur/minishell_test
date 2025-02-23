/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:34:01 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/16 22:34:01 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Close all pipes
void close_all_pipes(int num_pipes, int **pipes) {

    if (pipes == NULL) {
        return;
    }

    for (int i = 0; i < num_pipes; i++) {
        if (pipes[i] != NULL) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        } 
    }
}

// Free all pipes
void	free_pipes(int nbr_of_pipe, int **pipes)
{
	int	i;

	i = 0;
	while (i < nbr_of_pipe)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

// Count the number of commands in the parser
int	count_commands(t_parser *parser)
{
	int	count;

	count = 0;
	while (parser)
	{
		count++;
		parser = parser->next;
	}
	return (count);
}

// Execute the command in the child process
void	exec_child_command_exec(t_parser *curr, t_tools *tools)
{
	char	*path;
	char	**args;
	char	**local_envp;

	if (curr->redirects && process_redirections_child
		(tools, curr->redirects) != 0)
		exit(EXIT_FAILURE);
	if (!curr->tokens || !curr->tokens->str)
		exit(0);
	path = get_command_path(curr->tokens->str, tools->env);
	if (!path)
		(fprintf(stderr, "minishell: %s: command not found\n", \
		curr->tokens->str), exit(127));
	args = build_args(curr->tokens);
	if (!args)
	{
		free(path);
		exit(EXIT_FAILURE);
	}
	local_envp = copy_envp_to_execve(tools->env);
	execve(path, args, local_envp);
	perror("minishell");
	free(path);
	free_args(args);
	exit(EXIT_FAILURE);
}
