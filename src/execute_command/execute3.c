/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:50:51 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:50:51 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Fork all commands in the pipeline
int	fork_all_commands(t_tools *tools,
	t_parser *parser, int **pipes, pid_t *child_pids)
{
	int			i;
	int			num_cmd;
	t_parser	*curr;

	num_cmd = count_commands(parser);
	i = 0;
	curr = parser;
	while (curr)
	{
		child_pids[i] = fork();
		if (child_pids[i] == -1)
			return (-1);
		else if (child_pids[i] == 0)
		{
			setup_child_pipes(i, num_cmd, pipes);
			exec_child_command_exec(curr, tools);
		}
		else if (num_cmd > 1)
			close_parent_pipe_ends(i, num_cmd, pipes);
		i++;
		curr = curr->next;
	}
	return (0);
}

// Close unused pipe ends in the parent process
void	close_parent_pipe_ends(int i, int num_cmd, int **pipes)
{
	if (i > 0)
		close(pipes[i - 1][0]);
	if (i < num_cmd - 1)
		close(pipes[i][1]);
}

// Allocate memory for pipes and child PIDs
int	setup_mult_pipe(int num_cmd,
	int nbr_of_pipe, pid_t **child_pids, int ***pipes)
{
	*child_pids = malloc(num_cmd * sizeof(pid_t));
	if (!*child_pids)
		return (-1);
	if (nbr_of_pipe > 0)
	{
		*pipes = alloc_pipes(nbr_of_pipe);
		if (!*pipes)
		{
			free(*child_pids);
			return (-1);
		}
	}
	else
		*pipes = NULL;
	return (0);
}

// Wait for all child processes to finish
int	wait_for_all_children(int num_cmd, pid_t *child_pids)
{
	int		i;
	int		status;
	int		last_status;
	pid_t	wpid;

	i = 0;
	last_status = 0;
	while (i < num_cmd)
	{
		wpid = waitpid(child_pids[i], &status, 0);
		if (wpid == child_pids[num_cmd - 1])
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}
static int **setup_pipes(int num_pipes) {
    int **pipes = malloc(sizeof(int *) * num_pipes);
    if (!pipes) {
        perror("malloc pipes error");
        return NULL;
    }


    for (int i = 0; i < num_pipes; i++) {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i]) {
            perror("malloc pipe ends error");
            for (int j = 0; j < i; j++) {
                free(pipes[j]);
            }
            free(pipes);
            return NULL;
        }
        if (pipe(pipes[i]) == -1) {
            perror("pipe creation error");
            for (int j = 0; j <= i; j++) {
                free(pipes[j]);
            }
            free(pipes);
            return NULL;
        }
    }
    return pipes;
}

// Main function for executing multiple piped commands
//# Correct if_mult_pipe logic and setup_pipes function

 // Make sure to include your header file


static void free_tools_and_exit(t_tools *tools, t_parser *parser_list, t_lexer *lexer_list, int status)
{
	if(parser_list || lexer_list)
		printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n\n\n\n\n");
	free_lexer(&(tools->lexer));
	free_parser_list(&(tools->parser));
	tools = NULL;
    exit(status); // Terminate the child process with the given status
}
static pid_t fork_child(int command_index, int command_count, int **pipes, t_parser *parser, t_tools *tools)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        return -1; // Return -1 to indicate fork failure
    }

    if (pid == 0) // Child process
    {

        set_signals(); // Set signal handlers for child process

        if (command_count > 1) // Piping setup needed for multi-command pipeline
        {
            setup_child_pipes(command_index, command_count, pipes); // Redirect stdin/stdout
        }
        exec_child_command_exec(parser, tools); // Execute the command
        free_tools_and_exit(tools, tools->parser, tools->lexer, 127); // Exit child process after command execution (or if execve fails)

    }
    return pid; // Parent process returns the child PID
}


int if_mult_pipe(t_tools *tools, t_parser *parser, int num_pipes)
{
    int     **pipes;
    pid_t   *pids;
    int     command_count;
    int     status;


    pipes = setup_pipes(num_pipes);
    if (!pipes) {
        perror("pipe setup error");
        return (1);
    }

    pids = malloc(sizeof(pid_t) * (num_pipes + 1));
    if (!pids) {
        free_pipes(num_pipes, pipes);
        perror("malloc pids error");
        return (1);
    }

    command_count = 0;
    t_parser *curr_parser = parser; // Use a local pointer to traverse parser list
    while (curr_parser != NULL && curr_parser->is_pipeline_segment) // Continue while pipeline segments exist
    {
        pids[command_count] = fork_child(command_count, num_pipes + 1, pipes, curr_parser, tools);
        if (pids[command_count] == -1) {
            free(pids);
            free_pipes(num_pipes, pipes);
            return (1); // Fork error
        }

        curr_parser = curr_parser->next; // Move to next parser node in pipeline
        command_count++;
        if (command_count > num_pipes + 1) { // Safety check to prevent infinite loop if parser chain is incorrect
            break;
        }
    }

    close_all_pipes(num_pipes, pipes); // Parent closes all pipe ends

    status = wait_for_children(pids, command_count);

    free(pids);
    free_pipes(num_pipes, pipes);
    return (status);
}