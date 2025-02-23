/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe31.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:33:01 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/16 22:33:01 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int open_file_by_token(const char *filename, int token, t_tools *tools)
{
    int fd;

    if (token == TOKEN_REDIRECT_IN)
        return (open(filename, O_RDONLY));
    else if (token == TOKEN_REDIRECT_OUT)
        return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
    else if (token == TOKEN_APPEND)
        return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
    else if (token == TOKEN_HEREDOC)
    {
        char **delimiter_array = malloc(sizeof(char *) * 2);
        if (!delimiter_array)
        {
            perror("malloc");
            return (-1);
        }
        delimiter_array[0] = strdup(filename);
        if (!delimiter_array[0])
        {
            free(delimiter_array);
            return (-1);
        }
        delimiter_array[1] = NULL;

        // Call your heredoc handler, which returns the path of a temporary file.
        char *tmp_file = handle_heredoc_case(delimiter_array, tools, NO_QUOTE);
        
        // Free the delimiter array.
        free(delimiter_array[0]);
        free(delimiter_array);
        
        if (!tmp_file)
        {
            fprintf(stderr, "minishell: heredoc error\n");
            return (-1);
        }
        
        fd = open(tmp_file, O_RDONLY);
        free(tmp_file);
        return (fd);
    }
    fprintf(stderr, "minishell: syntax error near unexpected token\n");
    return (-1);
}



// Get the file descriptor for redirection
// Modify get_redirection_fd to accept t_tools *tools
int	get_redirection_fd(t_lexer *current, t_tools *tools)
{
	char	*filename;
	int		fd;

	if (!current->next || current->next->token != TOKEN_WORD)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token\n");
		return (-1);
	}
	filename = current->next->str;
	if (!filename)
	{
		fprintf(stderr, "minishell: error processing redirection\n");
		return (-1);
	}
	if (current->token == TOKEN_HEREDOC)
	{
		// Use the heredoc function to process input.
		// Here, we pass the delimiter (i.e. filename) as an array of one element.
		char *tmp_file = handle_heredoc_case(&filename, tools, NO_QUOTE);
		if (!tmp_file)
		{
			fprintf(stderr, "minishell: heredoc error\n");
			return (-1);
		}
		fd = open(tmp_file, O_RDONLY);
		free(tmp_file);
	}
	else
		fd = open_file_by_token(filename, current->token,tools);
	if (fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	return (fd);
}

// Process redirections in the child process
int	process_redirections_child(t_tools *tools, t_lexer *redirects)
{
	t_lexer	*current;
	int		fd;

	current = redirects;
	while (current)
	{
		fd = get_redirection_fd(current, tools);
		if (fd == -1)
			return (-1);
		if (dup_fd_for_token(fd, current->token) == -1)
		{
			close(fd);
			return (-1);
		}
		close(fd);
		// Move two tokens ahead (redirection operator and its target)
		current = current->next->next;
	}
	return (0);
}

// Allocate pipes for the pipeline
int	**alloc_pipes(int nbr_of_pipe)
{
	int		**pipes;
	int		i;

	pipes = malloc(sizeof(int *) * nbr_of_pipe);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < nbr_of_pipe)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) < 0)
		{
			perror("minishell: pipe");
			while (i-- > 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

// Set up pipes for the child process
// Corrected: Set up pipes for the child process
void setup_child_pipes(int command_index, int command_count, int **pipes) {
    printf("DEBUG: Entering setup_child_pipes, command_index: %d, command_count: %d, PID: %d\n",
           command_index, command_count, getpid());

    if (command_count > 1) {
        int pipe_count = command_count - 1; // there are always command_count-1 pipe pairs
        // Set up redirection for the current command:
        if (command_index == 0) { 
            // First command: redirect stdout to the write end of the first pipe.
            printf("DEBUG: setup_child_pipes - First command (index: %d), redirecting stdout to pipe[0][1]: %d\n",
                   command_index, pipes[0][1]);
            dup2(pipes[0][1], STDOUT_FILENO);
        } else if (command_index == command_count - 1) { 
            // Last command: redirect stdin from the read end of the last pipe.
            printf("DEBUG: setup_child_pipes - Last command (index: %d), redirecting stdin from pipe[%d][0]: %d\n",
                   command_index, command_index - 1, pipes[command_index - 1][0]);
            dup2(pipes[command_index - 1][0], STDIN_FILENO);
        } else { 
            // Middle command: read from previous pipe, write to next pipe.
            printf("DEBUG: setup_child_pipes - Middle command (index: %d), redirecting stdin from pipe[%d][0]: %d and stdout to pipe[%d][1]: %d\n",
                   command_index, command_index - 1, pipes[command_index - 1][0],
                   command_index, pipes[command_index][1]);
            dup2(pipes[command_index - 1][0], STDIN_FILENO);
            dup2(pipes[command_index][1], STDOUT_FILENO);
        }

        /* Now close all pipe file descriptors inherited by this child.
           This is important because if any write end remains open in any child,
           the last command (wc -l) will not receive an EOF and will count extra lines. */
        for (int i = 0; i < pipe_count; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
    } else {
        printf("DEBUG: setup_child_pipes - No piping (single command), nothing to do.\n");
    }

    printf("DEBUG: Exiting setup_child_pipes, PID: %d\n", getpid());
}


