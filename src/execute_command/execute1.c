/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:00:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/04 16:13:45 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_pipe_len(t_parser *parser)
{
	int		pipe_count; // Changed from 'i' to 'pipe_count' for clarity
	t_lexer	*current_token; // To iterate through tokens in the current command

	pipe_count = 0;
	if (!parser || !parser->tokens) // Check if parser or its tokens are NULL
		return (0);

	current_token = parser->tokens; // Start from the tokens of the *current* parser node
	while (current_token != NULL)   // Iterate through the tokens of THIS command
	{
		if (current_token->token == TOKEN_PIPE) // Check for PIPE token
			pipe_count++;                    // Increment pipe counter
		current_token = current_token->next;   // Move to the next token
	}
	return (pipe_count); // Return the count of pipes in THIS command
}

// static int is_empty_parser_node(t_parser *node)
// {
//     t_lexer *tok;

//     if (!node || !node->tokens)
//         return (1);
//     tok = node->tokens;
//     while (tok)
//     {
//         if (tok->token == TOKEN_WORD && tok->str && tok->str[0] != '\0')
//             return (0);
//         tok = tok->next;
//     }
//     return (1);
// }

static int count_pipeline_segments(t_parser *start_parser) {
    int count = 0;
    t_parser *current = start_parser;

    

    // 1. Count pipeline segments from the START (assuming start_parser IS the start)
    while (current != NULL && current->is_pipeline_segment) {
        count++;
        current = current->next;
    }

    return (count > 0 ? count - 1 : 0); // Number of pipes is segments - 1 (or 0 if no segments)
}


// Helper function to count pipeline segments in a chain starting from a given parser node

int execute_command(t_tools *tools, char **envp) {
    (void)envp; // Mark envp as intentionally unused in this function
    t_parser *current_parser = tools->parser;
    int last_status = 0;

    while (current_parser != NULL) {
        if (current_parser->tokens) {
            t_lexer *token_debug = current_parser->tokens;
            while (token_debug) {
                token_debug = token_debug->next;
            }
        } 
        if (current_parser->is_pipeline_segment) {
            set_execution_signals();
            // *** Calculate TOTAL pipes ONCE at pipeline START ***
            int total_pipes = count_pipeline_segments(current_parser); // Call count_pipeline_segments ONCE with pipeline START

            last_status = if_mult_pipe(tools, current_parser, total_pipes); // Call if_mult_pipe for the pipeline
            set_signals();

            // Advance current_parser to the node AFTER the pipeline
            t_parser *pipeline_end = current_parser;
            int pipeline_segment_count = 0;
            while (pipeline_end != NULL && pipeline_end->is_pipeline_segment) {
                pipeline_end = pipeline_end->next;
                pipeline_segment_count++;
            }
            current_parser = pipeline_end;
        } else {
            set_execution_signals();
            last_status = if_no_pipe(tools, current_parser, envp);
            set_signals();
            current_parser = current_parser->next; // Move to the next command
        }
    }

    return (last_status); // Final return
}