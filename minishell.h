/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:02:45 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/04 15:56:44 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <errno.h>
# include <stdbool.h>
# include <signal.h>
# include "libft/libft.h"

#define NO_DELIM       0
#define DELIM_PIPE     TOKEN_PIPE
#define DELIM_SEMICOLON TOKEN_SEMICOLON

struct	s_parser;
struct	s_env;
typedef int	(*t_builtin_func)(struct s_parser *, struct s_env **);

typedef enum e_tokens
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_SEMICOLON,
	TOKEN_EOF
}	t_tokens;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTES,
	DOUBLE_QUOTES,
	QUOTE_ERROR
}	t_quote_type;

typedef struct s_lexer
{
	char			*str;
	int				i;
	t_tokens		token;
	t_quote_type	quote_type;
	int				space;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				hidden;
	struct s_env	*next;
	int				two_point;
}	t_env;

typedef struct s_parser
{
	t_lexer				*tokens;
	t_builtin_func		builtin;
	t_lexer				*redirects;
	char				**hd_delimiters;
	int              delim;
	int is_pipeline_segment;
	struct s_parser		*next;
	struct s_parser		*prev;
}	t_parser;

typedef struct s_tools
{
	int			last_exit_status;
	t_lexer		*lexer;
	t_parser	*parser;
	t_env		*env;
}	t_tools;

typedef struct s_heredoc_ctx
{
	int		stdin_copy;
	int		tmp_fd;
	char	*delimiter;
	char	template[32];
	int		expand_variables;
}	t_heredoc_ctx;

typedef struct s_exp_ctx
{
	char		*expanded;
	const char	*line;
	size_t		i;
	size_t		j;
	t_env		*env;
} t_exp_ctx;

extern int	g_signum;

void			art(void);
void			handle_heredoc_signal(int sig);
int				builtin_env(t_parser *parser, t_env **env);
int				builtin_pwd(t_parser *parser, t_env **env);
int				builtin_exit(t_parser *parser, t_env **env);
int				builtin_unset(t_parser *parser, t_env **env);
int				builtin_export(t_parser *parser, t_env **env);
int				builtin_cd(t_parser *parser, t_env **env);
char			*get_env(t_env *head, const char *key);
int				ft_cmp(const char *s1, const char *s2);
int				get_cmd_len(t_lexer *lexer);
char			**get_cmd(t_lexer *lexer);
int				is_numeric(const char *str);
void			add_env_var(t_env **env, const char *key, const char *value);
void			update_env_var(t_env **env, const char *key, const char *value);
void			print_full_export_list(t_env *env);
char			**export_split(const char *str, const char *delimiter);
void			free_split_array(char **array);
t_env			*create_env_node(char *key, char *value, int hidden);
int				set_env(t_env **head, const char *key,
					const char *value, int hidden);
void			add_lexer_node_back(t_lexer **head, t_lexer *new_node);
t_lexer			*create_new_lexer_node(char *str, t_tokens token,
					t_quote_type quote_type, int space);
void			handle_normal_string(t_lexer **lexer, const char *str, int *i);
int				special_char(char c);
int				whitespacee(char c);
void			handle_pipe(t_lexer **lexer, int *i);
void			handle_pipe11(const char *s, int *i, t_lexer **lexer_list);
void			handle_in_or_heredoc(t_lexer **lexer, const char *str, int *i);
void			handle_out_or_append(t_lexer **lexer, const char *str, int *i);
int				get_quote_length(const char *str, int i, char quote);
void			handle_quotes(t_lexer **lexer,
					const char *str, int *i, char quote);
void			add_index_token(t_lexer **lexer);
void			parse_string(t_lexer **lexer, const char *str);
void			skip_spaces(const char *str, int *i);
void			handle_special_char(t_lexer **lexer, const char *str, int *i);
void			process_string(t_lexer **lexer, const char *str, int *i);
void			add_lexer_and_increment_i(t_lexer **lexer,
					int token, int *i, int increment);
void			fill_sub_lexer(t_lexer **sub,
					t_lexer **tmp, unsigned int sub_size);
t_lexer			*find_start_node(t_lexer *lexer, t_lexer *start_node);
t_lexer			*ft_sublexer(t_lexer *lexer,
					t_lexer *start_node, unsigned int nb_nodes);
void			free_env(t_env **env);
void			check_signal_if_recieved(int *status);
void			handle_execution_signals(int sig);
void			handle_ctrl_c(int sig);
void			ignore_signals(void);
void			set_ctrl_d(t_tools *tools, char *prompt);
void			set_execution_signals(void);
void			reset_signals(void);
void			set_signals(void);
int				execute_command(t_tools *tools, char **envp);
int				if_no_pipe(t_tools *tools, t_parser *parser, char **envp);
char			*get_command_path(char *cmd, t_env *env);
char			**build_args(t_lexer *tokens);
int				process_redirections_child(t_tools *tools, t_lexer *redirects);
char			*my_strtok(char *str, const char *delim);
int				if_mult_pipe(t_tools *tools, t_parser *parser, int num_pipes);
int				process_all_heredocs(t_parser *parser, t_tools *tools);
int				check_unclosed_quotes(char *str);
void			set_one_builtin(t_parser *curr);
void			set_builtins(t_tools *tools);
int				ft_strcmp1(const char *s1, const char *s2);
char			**copy_envp_to_execve(t_env *env);
void			fill_missing_strings(t_lexer *sub);
void			free_lexer(t_lexer **lexer);
int				count_pipes(t_lexer *node);
void			free_parser_list(t_parser **head);
void			free_parser_content(t_parser *node);
void			delete_parser_node(t_parser **head, t_parser *node_to_delete);
int				validate_pipes_and_words(t_parser *head);
int				is_redirection_token(t_tokens token);
int				handle_redirect_in(t_lexer *current);
int				check_invalid_token(t_lexer *current);
int				check_redirections(t_lexer *lexer);
int				check_parser_redirections(t_parser *parser);
void			print_syntax_error(char *msg, int *status);
t_parser		*create_new_parser_node(void);
void			parser_add_back(t_parser **parser);
void			init_parser_node(t_parser **parser,
					t_lexer *lexer, const char *str);
void			add_to_redirects(t_lexer **redirects, t_lexer *redirection,
					t_lexer *target);
void			init_one_redirect(t_lexer **redirects, t_lexer **lexer);
void			init_redirections(t_parser *parser);
void			free_lexer_node(t_lexer **lexer, t_lexer *to_del);
void			set_hd_limiter_parser(t_parser *parser);
void			set_hd_limiters_in_node(t_parser *node);
int				get_nb_of_heredocs(t_lexer *redirs);
char			*get_token(t_tokens token);
void			set_tokens_lexer(t_parser **parser, t_lexer *lexer);
void			ft_tokens_join(t_lexer **tmp, t_lexer **to_del);
void			free_strs_nodes(char **s1, char **s2);
void			delete_empty_node(t_lexer **tokens, t_lexer **tmp, char **str);
void			update_parser_tokens(t_tools *tools);
void			update_token_words(t_tools *tools, t_lexer **current);
void			cat_words(t_parser *parser);
void			cat_heredoc_word(t_lexer **lexer, t_lexer *current);
void			cat_heredoc_words(t_parser *parser);
int				main(int argc, char *argv[], char *envp[]);
void			set_init(int argc, char *argv[], int *status, t_env **env);
char			*minishell_loop(int status);
void			set_env_var(t_env **env, char *key, char *value, int hidden);
void			copy_envp(t_env **env, char **envp);
void			update_shlvl(t_env **env, int hidden);
void			initialize_environment(t_env **env);
int				init_tools(t_tools *tools, char *str, char **envp);
void			free_tools(t_tools *tools);
int				check_initial_errors(char *prompt, t_tools *tools);
void			print_message_error(char *s1, char *s2, char *s3, char *s4);
int				ft_isspacee(char *str);
void			env_add_back(t_env **env, char *key, char *value, int hidden);
t_env			*get_env_var(t_env *env, char *key);
void			free_split(char **split);
int				builtin_echo(t_parser *parser, t_env **env);
void			update_double_quotes_case(t_tools *tools, t_lexer *current);
void			insert_at(t_lexer **lexer,
					t_lexer *current, char *str, int space);
void			update_no_quotes_case(t_tools *tools,
					t_lexer **current, t_lexer **tmp);
char			*string_converter(t_lexer *current, t_tools *tools);
void			handle_dollar_sign_str(t_lexer *current, t_tools *tools,
					int *i, char **result);
pid_t			get_pid(void);
void			handle_simple_string(t_lexer *to_del,
					char ***split, char **str);
void			handle_empty_split(t_lexer **current, t_lexer *to_del,
					char ***split, char **str);
int				get_new_string_length(t_lexer *current, t_tools *tools);
void			handle_question_mark(t_tools *tools,
					char **sub, int **i, int **len);
void			handle_dollar_sign_len(t_lexer *current, t_tools *tools,
					int *i, int *len);
int				is_a_special_case(t_lexer *current, int *i);
void			fill_ptr_substr(char **ptr, char *buffer, pid_t *pid);
void			handle_dollar_special_case(t_lexer	*current, char **sub,
					int **len, int **i);
char			*handle_heredoc_case(char **delimiters, t_tools *tools,
					t_quote_type quote_type);
int				open_file(char *filename, int oflags, int access);
int				get_delimiters_count(char **delimiters);
int				get_nb_of_pipes(t_lexer *lexer);
char			*ft_strndupp(const char *src, size_t n);
char			*ft_strstr(const char *haystack, const char *needle);
char			*remove_escaped_quotes(const char *str);
void			handle_special_char(t_lexer **lexer, const char *str, int *i);
void			handle_semicolon(t_lexer **lexer, int *i);
t_quote_type	get_quote_type(char quote);
int				process_heredoc_fd(t_tools *tools, t_lexer *current);
int				open_fd_for_redirection(t_tools *tools, t_lexer *current);
int				dup_fd_for_token(int fd, int token);
int				execute_builtin_no_pipe(t_tools *tools, t_parser *parser);
void			free_args(char **args);
void			execute_external_command(t_tools *tools, t_parser *parser);
void			validate_and_update_tokens(t_parser *parser);
void			child_process_no_pipe(t_tools *tools, t_parser *parser);
//int				wait_for_single_child(pid_t pid, int);
int				open_file_by_token(const char *filename, int token, t_tools *tools);
int				get_redirection_fd(t_lexer *current, t_tools *tools);
int				process_redirections_child(t_tools *tools, t_lexer *redirects);
int				**alloc_pipes(int nbr_of_pipe);
void			setup_child_pipes(int i, int num_cmd, int **pipes);
void			exec_child_command_exec(t_parser *curr, t_tools *tools);
int				fork_all_commands(t_tools *tools,
					t_parser *parser, int **pipes, pid_t *child_pids);
void			close_parent_pipe_ends(int i, int num_cmd, int **pipes);
int				setup_mult_pipe(int num_cmd,
					int nbr_of_pipe, pid_t **child_pids, int ***pipes);
int				wait_for_all_children(int num_cmd, pid_t *child_pids);
void			close_all_pipes(int nbr_of_pipe, int **pipes);
void			free_pipes(int nbr_of_pipe, int **pipes);
int				count_commands(t_parser *parser);
int				process_redirections_child_for_no_pipe(t_tools *tools, t_lexer *redirects);
char			*find_path_env(t_env *env);
int				get_nb_of_semicolon(t_lexer *lexer);
void			print_heredoc_error(char *delimiter);
int				handle_sigint(t_heredoc_ctx *ctx, char *line);
int				process_heredoc_line(t_heredoc_ctx *ctx, t_tools *tools, char *line);
int				heredoc_loop(t_heredoc_ctx *ctx, t_tools *tools);
char			*expand_heredoc_line(char *line, t_env *env, int last_ret, int expand_variables);
void			expand_exit_status(t_exp_ctx *ctx, int last_ret);
void			expand_env_variable(t_exp_ctx *ctx);
int wait_for_children(pid_t *pids, int command_count);
int wait_for_single_child(pid_t pid) ;
#endif