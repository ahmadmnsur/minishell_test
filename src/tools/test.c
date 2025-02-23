/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:00:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 16:00:51 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// void print_parser_list(t_parser *parser_list) {
//     t_parser *current_parser = parser_list;
//     int i = 0;

//     while (current_parser != NULL) {
//         t_lexer *token_debug = current_parser->tokens;
//         while (token_debug != NULL) {
//             token_debug = token_debug->next;
//         }
//         printf("\n");
//         current_parser = current_parser->next;
//         i++;
//     }
//     printf("DEBUG: End of Parser List\n");
// }

int	init_tools(t_tools *tools, char *str, char **envp)
{
	tools->lexer = NULL;
	tools->parser = NULL;
	(void)envp;
	parse_string(&(tools->lexer), str);
	init_parser_node(&(tools->parser), tools->lexer, str);
	// print_parser_list(tools->parser);
	// printf("DEBUG: After assign_tokens_to_parser in set_tokens_lexer\n");
	set_tokens_lexer(&(tools->parser), tools->lexer);
	if (!check_initial_errors(str, tools))
		return (free(str), 0);
	free(str);
	free_lexer(&(tools->lexer));
	update_parser_tokens(tools);
	//printf("after update parser token\n");
	//print_parser_debug_info(tools->parser);
	init_redirections(tools->parser);
	set_hd_limiter_parser(tools->parser);
	set_builtins(tools);
	return (1);
}

// void init_parser_node(t_parser **parser, t_lexer *lexer, const char *str) {
//     t_lexer         *current_lexer_token;
//     t_parser        *current_parser_node;
//     t_parser        *new_parser_node;

//     if (!str || !str[0] || !lexer) {
//         printf("DEBUG: Invalid input string or lexer\n");
//         return;
//     }

//     // 1. Initialize parser list and first node
//     *parser = create_new_parser_node();
//     if (!*parser) {
//         printf("DEBUG: Failed to create new parser node\n");
//         return;
//     }
//     current_parser_node = *parser;
//     current_lexer_token = lexer;

//     printf("DEBUG: Entering revised init_parser_node (with token assign)\n");
//     printf("DEBUG: init_parser_node - Initial: parser = %p, *parser = %p, current_parser_node = %p\n", parser, *parser, current_parser_node);

//     // 2. Iterate through lexer tokens and assign tokens to parser nodes DURING creation
//     while (current_lexer_token && current_lexer_token->token != TOKEN_EOF) {
//         printf("DEBUG: init_parser_node (token assign) - Processing lexer token: %d (\"%s\")\n",
//                current_lexer_token->token, current_lexer_token->str ? current_lexer_token->str : "");

//         if (current_lexer_token->token == TOKEN_PIPE || current_lexer_token->token == TOKEN_SEMICOLON) {
//             // Check if the delimiter is trailing or if the next token is another delimiter.
//             if (current_lexer_token->token == TOKEN_PIPE && (!current_lexer_token->next || current_lexer_token->next->token == TOKEN_EOF)) {
//                 // Trailing PIPE is a syntax error
//                 printf("DEBUG: Found trailing PIPE - SYNTAX ERROR\n");
//                 break; // Exit loop for syntax error (TRAILING PIPE) - BREAK ONLY FOR PIPE!
//             } else if (current_lexer_token->token == TOKEN_SEMICOLON && (!current_lexer_token->next || current_lexer_token->next->token == TOKEN_EOF)) {
//                 // Trailing SEMICOLON - NOT a syntax error, just end of command sequence for current node
//                 printf("DEBUG: Found trailing SEMICOLON - NOT a syntax error, continuing\n");
//                 // DO NOT break; just continue to create a new node and move on.
//             }
//             if (current_lexer_token->token == TOKEN_PIPE) {
//                 printf("DEBUG: init_parser_node - PIPE token: parser = %p, *parser = %p, current_parser_node = %p\n", parser, *parser, current_parser_node);
//                 printf("DEBUG: init_parser_node - PIPE token BEFORE LINKING: current_parser_node->next = %p\n", current_parser_node->next); // Debug BEFORE linking
    
//                 current_parser_node->is_pipeline_segment = 1;
//                 new_parser_node = create_new_parser_node();
//                 if (!new_parser_node)
//                     return;
//                 new_parser_node->is_pipeline_segment = 0; 
    
//                 current_parser_node->next = new_parser_node; // LINKING - Set next pointer
    
//                 printf("DEBUG: init_parser_node - PIPE token AFTER LINKING: current_parser_node->next = %p, new_parser_node = %p\n", current_parser_node->next, new_parser_node); // Debug AFTER linking and new_node address
    
//                 current_parser_node = new_parser_node;
//                 printf("DEBUG: Created NEW parser node and moved to it\n");
//                 printf("DEBUG: init_parser_node - After new node: parser = %p, *parser = %p, current_parser_node = %p\n", parser, *parser, current_parser_node);
    
//                 current_lexer_token = current_lexer_token->next; 
//                 continue; 
//             } 
//           else { // TOKEN_SEMICOLON
//                 // Do not override the flag of the current node if it was already part of a pipeline.
//                 printf("DEBUG: Found SEMICOLON, creating NEW parser node for command sequence\n");
//             }

//             new_parser_node = create_new_parser_node();
//             if (!new_parser_node)
//                 return;
//             // For a pipe, new node is part of the pipeline; for semicolon, it's a separate command.
//             new_parser_node->is_pipeline_segment = 0; // Semicolon-separated command is NOT a pipeline segment

//             current_parser_node->next = new_parser_node;
//             current_parser_node = new_parser_node;
//             printf("DEBUG: Created NEW parser node and moved to it\n");
//             printf("DEBUG: init_parser_node - After new node: parser = %p, *parser = %p, current_parser_node = %p\n", parser, *parser, current_parser_node);
//         } else {
//             // Add token to current node.
//             printf("DEBUG: Adding token to current parser node: %d (\"%s\")\n",
//                    current_lexer_token->token,
//                    current_lexer_token->str ? current_lexer_token->str : "");
//             add_lexer_node_back(&(current_parser_node->tokens),
//                                 create_new_lexer_node(ft_strdup(current_lexer_token->str),
//                                                       current_lexer_token->token,
//                                                       current_lexer_token->quote_type,
//                                                       current_lexer_token->space));
//         }

//         current_lexer_token = current_lexer_token->next;
//     }
//     printf("DEBUG: init_parser_node - After loop: parser = %p, *parser = %p, current_parser_node = %p\n", parser, *parser, current_parser_node);
//     printf("DEBUG: Exiting revised init_parser_node (with token assign)\n");
// }

void init_parser_node(t_parser **parser, t_lexer *lexer, const char *str) {
    t_lexer         *current_lexer_token;
    t_parser        *current_parser_node;
    t_parser        *new_parser_node;

    if (!str || !str[0] || !lexer) {
        return;
    }

    // 1. Initialize parser list and first node
    *parser = create_new_parser_node();
    if (!*parser) {
        return;
    }
    current_parser_node = *parser;
    current_lexer_token = lexer;


    // 2. Iterate through lexer tokens and assign tokens to parser nodes DURING creation
    while (current_lexer_token && current_lexer_token->token != TOKEN_EOF) {

        if (current_lexer_token->token == TOKEN_PIPE || current_lexer_token->token == TOKEN_SEMICOLON) {
            // Check if the delimiter is trailing or if the next token is another delimiter.
            if (current_lexer_token->token == TOKEN_PIPE && (!current_lexer_token->next || current_lexer_token->next->token == TOKEN_EOF)) {
                break; // Exit loop for syntax error (TRAILING PIPE) - BREAK ONLY FOR PIPE!
            } 
            if (current_lexer_token->token == TOKEN_PIPE) {
                current_parser_node->is_pipeline_segment = 1;
            }

            new_parser_node = create_new_parser_node();
            if (!new_parser_node)
                return;
            // For a pipe, new node is part of the pipeline; for semicolon, it's a separate command.
            new_parser_node->is_pipeline_segment = (current_lexer_token->token == TOKEN_PIPE) ? 1 : 0;

            current_parser_node->next = new_parser_node;
            current_parser_node = new_parser_node;
        } else if (is_redirection_token(current_lexer_token->token)) { // NEW: Redirection Handling
            t_lexer *redirection_token = current_lexer_token;
            current_lexer_token = current_lexer_token->next; // Move to filename token

            if (!current_lexer_token || current_lexer_token->token != TOKEN_WORD) { // Error: Filename missing./
                // Handle syntax error (e.g., break loop, set error flag)
                break;
            }
            t_lexer *filename_token = current_lexer_token;

            add_to_redirects(&(current_parser_node->redirects), redirection_token, filename_token); // Add to redirects list
            // DO NOT add redirection_token or filename_token to current_parser_node->tokens

            current_lexer_token = current_lexer_token->next; // Advance PAST filename token - IMPORTANT!
            continue; // Continue to next lexer token
        }
         else {
            // Add token to current node.
            add_lexer_node_back(&(current_parser_node->tokens),
                                 create_new_lexer_node(strdup(current_lexer_token->str), // Use strdup
                                                         current_lexer_token->token,
                                                         current_lexer_token->quote_type,
                                                         current_lexer_token->space));
        }

        current_lexer_token = current_lexer_token->next;
    }
}

//*********************         ha sa777777777 yle fo2  */

// void init_parser_node(t_parser **parser, t_lexer *lexer, const char *str) {
//     t_lexer         *current_lexer_token;
//     t_parser        *current_parser_node;
//     t_parser        *new_parser_node;

//     if (!str || !str[0] || !lexer) {
//         printf("DEBUG: Invalid input string or lexer\n");
//         return;
//     }

//     // 1. Initialize parser list and first node
//     *parser = create_new_parser_node();
//     if (!*parser) {
//         printf("DEBUG: Failed to create new parser node\n");
//         return;
//     }
//     current_parser_node = *parser;
//     current_lexer_token = lexer;

//     printf("DEBUG: Entering revised init_parser_node (with token assign)\n");

//     // 2. Iterate through lexer tokens and assign tokens to parser nodes DURING creation
//     while (current_lexer_token && current_lexer_token->token != TOKEN_EOF) {
//         printf("DEBUG: init_parser_node (token assign) - Processing lexer token: %d (\"%s\")\n",
//                current_lexer_token->token, current_lexer_token->str ? current_lexer_token->str : "");

//         if (current_lexer_token->token == TOKEN_PIPE || current_lexer_token->token == TOKEN_SEMICOLON) {
//             // Check if the delimiter is trailing or if the next token is another delimiter.
// 			if (current_lexer_token->token == TOKEN_PIPE && (!current_lexer_token->next || current_lexer_token->next->token == TOKEN_EOF)) {
// 				// Trailing PIPE is a syntax error
// 				printf("DEBUG: Found trailing PIPE - SYNTAX ERROR\n");
// 				break; // Exit loop for syntax error (TRAILING PIPE) - BREAK ONLY FOR PIPE!
// 			} else if (current_lexer_token->token == TOKEN_SEMICOLON && (!current_lexer_token->next || current_lexer_token->next->token == TOKEN_EOF)) {
// 				// Trailing SEMICOLON - NOT a syntax error, just end of command sequence for current node
// 				printf("DEBUG: Found trailing SEMICOLON - NOT a syntax error, continuing\n");
// 				// DO NOT break; just continue to create a new node and move on.
// 			}
            
//             if (current_lexer_token->token == TOKEN_PIPE && (!current_lexer_token->next || current_lexer_token->next->token == TOKEN_EOF)) {
//                 current_parser_node->is_pipeline_segment = 1;
//                 printf("DEBUG: Found PIPE, creating NEW parser node for pipeline, setting CURRENT node as pipeline segment\n");
//             } else { // TOKEN_SEMICOLON
//                 // Do not override the flag of the current node if it was already part of a pipeline.
//                 printf("DEBUG: Found SEMICOLON, creating NEW parser node for command sequence\n");
//             }
            
//             new_parser_node = create_new_parser_node();
//             if (!new_parser_node)
//                 return;
//             // For a pipe, new node is part of the pipeline; for semicolon, it's a separate command.
//             new_parser_node->is_pipeline_segment = (current_lexer_token->token == TOKEN_PIPE) ? 1 : 0;
            
//             current_parser_node->next = new_parser_node;
//             current_parser_node = new_parser_node;
//             printf("DEBUG: Created NEW parser node and moved to it\n");
//         } else {
//             // Add token to current node.
//             printf("DEBUG: Adding token to current parser node: %d (\"%s\")\n",
//                    current_lexer_token->token,
//                    current_lexer_token->str ? current_lexer_token->str : "");
//             add_lexer_node_back(&(current_parser_node->tokens),
//                 create_new_lexer_node(ft_strdup(current_lexer_token->str),
//                                       current_lexer_token->token,
//                                       current_lexer_token->quote_type,
//                                       current_lexer_token->space));
//         }
        
//         current_lexer_token = current_lexer_token->next;
//     }
//     printf("DEBUG: Exiting revised init_parser_node (with token assign)\n");
// }

//ha yle fo2

// void init_parser_node(t_parser **parser, t_lexer *lexer, const char *str) {
//     t_lexer         *current_lexer_token;
//     t_parser        *current_parser_node;
//     t_parser        *new_parser_node;

//     if (!str || !str[0] || !lexer) {
//         printf("DEBUG: Invalid input string or lexer\n");
//         return;
//     }

//     // 1. Initialize parser list and first node
//     *parser = create_new_parser_node();
//     if (!*parser) {
//         printf("DEBUG: Failed to create new parser node\n");
//         return;
//     }
//     current_parser_node = *parser;
//     current_lexer_token = lexer;

//     printf("DEBUG: Entering revised init_parser_node (with token assign)\n");

//     // 2. Iterate through lexer tokens and assign tokens to parser nodes DURING creation
//     while (current_lexer_token && current_lexer_token->token != TOKEN_EOF) {
//         printf("DEBUG: init_parser_node (token assign) - Processing lexer token: %d (\"%s\")\n",
//                current_lexer_token->token,
//                current_lexer_token->str ? current_lexer_token->str : "");

//         if (current_lexer_token->token == TOKEN_PIPE || current_lexer_token->token == TOKEN_SEMICOLON) {
//             // Check for consecutive semicolons
//             if (current_lexer_token->token == TOKEN_SEMICOLON &&
//                 current_lexer_token->next && current_lexer_token->next->token == TOKEN_SEMICOLON) {
//                 printf("DEBUG: Syntax error: consecutive semicolons found\n");
//                 return;
//             }
//             // Check if the delimiter is trailing or if the next token is TOKEN_EOF.
//             if (!current_lexer_token->next || current_lexer_token->next->token == TOKEN_EOF) {
//                 printf("DEBUG: Found trailing delimiter, exiting loop\n");
//                 break;
//             }
            
//             if (current_lexer_token->token == TOKEN_PIPE) {
//                 current_parser_node->is_pipeline_segment = 1;
//                 printf("DEBUG: Found PIPE, creating NEW parser node for pipeline, setting CURRENT node as pipeline segment\n");
//             } else { // TOKEN_SEMICOLON
//                 // Do not override the current node's flag if it was already set as part of a pipeline.
//                 printf("DEBUG: Found SEMICOLON, creating NEW parser node for command sequence\n");
//             }
            
//             new_parser_node = create_new_parser_node();
//             if (!new_parser_node)
//                 return;
//             // For a pipe, new node is part of the pipeline; for semicolon, it's a separate command.
//             new_parser_node->is_pipeline_segment = (current_lexer_token->token == TOKEN_PIPE) ? 1 : 0;
            
//             current_parser_node->next = new_parser_node;
//             current_parser_node = new_parser_node;
//             printf("DEBUG: Created NEW parser node and moved to it\n");
//         } else {
//             // Add token to current node.
//             printf("DEBUG: Adding token to current parser node: %d (\"%s\")\n",
//                    current_lexer_token->token,
//                    current_lexer_token->str ? current_lexer_token->str : "");
//             add_lexer_node_back(&(current_parser_node->tokens),
//                 create_new_lexer_node(ft_strdup(current_lexer_token->str),
//                                       current_lexer_token->token,
//                                       current_lexer_token->quote_type,
//                                       current_lexer_token->space));
//         }
        
//         current_lexer_token = current_lexer_token->next;
//     }
//     printf("DEBUG: Exiting revised init_parser_node (with token assign)\n");
// }


int get_nb_of_semicolon(t_lexer *lexer)
{
    int count;
    t_lexer *current;

	current = lexer;
	count = 0;
    while (current)
    {
        if (current->token == TOKEN_SEMICOLON)
            count++;
        current = current->next;
    }
    return (count);
}

