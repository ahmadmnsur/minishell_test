/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:02:49 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 16:30:25 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fcntl.h"
#include "unistd.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*prompt;
	char	*cmd;
	t_tools	tools;

	art();
	set_init(argc, argv, &(tools.last_exit_status), &(tools.env));
	(copy_envp(&(tools.env), envp), initialize_environment(&(tools.env)));
	while (1)
	{
		prompt = minishell_loop(tools.last_exit_status);
		set_ctrl_d(&tools, prompt);
		check_signal_if_recieved(&(tools.last_exit_status));
		if (!ft_isspacee(prompt))
			add_history(prompt);
		cmd = ft_strdup(prompt);
		free(prompt);
		if (init_tools(&tools, cmd, envp))
			tools.last_exit_status = execute_command(&tools, envp);
        free_parser_list(&(tools.parser));
	}
	(free_env(&(tools.env)), rl_clear_history());
	return (0);
}

void	set_init(int argc, char *argv[], int *status, t_env **env)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("minishell: Too many arguments", STDERR_FILENO);
		exit(1);
	}
	*status = 0;
	*env = NULL;
	g_signum = 0;
	set_signals();
}

char	*minishell_loop(int status)
{
    char	*prompt;
    char	*last_status;
   // char	*formatted_prompt;
    char	*readline_;

    if (status == 0)
    {
        // Note: Wrap non-printable sequences with \001 and \002
        prompt = ft_strdup("\001\033[32m\002minishell\001\033[0m\002\001\033[34m\002[0]\001\033[0m\002$ ");
        readline_ = readline(prompt);
        free(prompt);
    }
    else
    {
        // Constructing the prompt dynamically with the status code.
        // Start with the colored 'minishell'
        prompt = ft_strdup("\001\033[32m\002minishell\001\033[0m\002");
        // Append the status with its color
        prompt = ft_strjoin(prompt, "\001\033[31m\002[");
        last_status = ft_itoa(status);
        prompt = ft_strjoin(prompt, last_status);
        free(last_status);
        prompt = ft_strjoin(prompt, "]\001\033[0m\002$ ");
        readline_ = readline(prompt);
        free(prompt);
    }
    return (readline_);
}

void	set_env_var(t_env **env, char *key, char *value, int hidden)
{
	t_env	*to_get;
	char	*new_val;

	if (hidden && get_env_var(*env, key))
		return ;
	if (!*env)
	{
		env_add_back(env, key, value, hidden);
		return ;
	}
	to_get = get_env_var(*env, key);
	if (!to_get)
		env_add_back(env, key, value, hidden);
	else
	{
		new_val = ft_strdup(value);
		free(to_get->value);
		to_get->value = new_val;
		to_get->hidden = hidden;
	}
}

void	copy_envp(t_env **env, char **envp)
{
	int		i;
	char	**split;

	if (!envp || !*envp)
		return ;
	i = 0;
	while (envp[i])
	{
		split = ft_split(envp[i], "=");
		set_env_var(env, split[0], split[1], 0);
		free_split(split);
		split = NULL;
		i++;
	}
	free_split(split);
	update_shlvl(env, 0);
}

void	update_shlvl(t_env **env, int hidden)
{
	char	*shlvl_str;
	int		shlvl;

	shlvl_str = get_env_var(*env, "SHLVL")->value;
	shlvl = ft_atoi(shlvl_str);
	shlvl++;
	shlvl_str = ft_itoa(shlvl);
	set_env_var(env, "SHLVL", shlvl_str, hidden);
	free(shlvl_str);
}
void	initialize_environment(t_env **env)
{
	char	cwd[1024];

	if (getenv("PWD"))
		return ;
	if (getcwd(cwd, sizeof(cwd)))
		set_env_var(env, "PWD", cwd, 0);
	else
		perror("minishell: getcwd");
	set_env_var(env, "OLDPWD", NULL, 1);
	set_env_var(env, "SHLVL", "1", 0);
}

void	free_tools(t_tools *tools)
{
	free_lexer(&(tools->lexer));
	free_parser_list(&(tools->parser));
	tools = NULL;
}

static int	validate_semicolon_and_words(t_parser *head)
{
	t_parser	*current;
	t_lexer		*lexer_node;

	if (head == NULL)
		return (1);
	current = head;
	while (current)
	{
		// If the parser node is empty (no tokens),
		// allow it only if it is the last command (trailing semicolon).
		if (current->tokens == NULL)
		{
			if (current->next != NULL)
				return (0);
		}
		else
		{
			int	found_word = 0;
			lexer_node = current->tokens;
			while (lexer_node)
			{
				if (lexer_node->token == TOKEN_WORD)
				{
					found_word = 1;
					break ;
				}
				lexer_node = lexer_node->next;
			}
			// If no word token was found and this is not the last node, it's an error.
			if (!found_word && current->next != NULL)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_initial_errors(char *prompt, t_tools *tools)
{
	char	c;

	// Check for unclosed quotes first.
	if (check_unclosed_quotes(prompt))
	{
		c = check_unclosed_quotes(prompt);
		if (c == '\'')
			print_syntax_error("'", &(tools->last_exit_status));
		else
			print_syntax_error("\"", &(tools->last_exit_status));
		return (free_tools(tools), 0);
	}

	// If the prompt contains a semicolon, validate semicolon usage.
	if (ft_strchr(prompt, ';') != NULL)
	{
		if (!validate_semicolon_and_words(tools->parser))
		{
			print_syntax_error(";", &(tools->last_exit_status));
			return (free_tools(tools), 0);
		}
	}

	// If the prompt contains a pipe, validate pipe usage.
	if (ft_strchr(prompt, '|') != NULL)
	{
		if (!validate_pipes_and_words(tools->parser))
		{
			print_syntax_error("|", &(tools->last_exit_status));
			return (free_tools(tools), 0);
		}
	}
	// If there are no pipe issues, check redirections.
	else if (!check_parser_redirections(tools->parser))
	{
		tools->last_exit_status = 2;
		return (free_tools(tools), 0);
	}
	return (1);
}

void	print_message_error(char *s1, char *s2, char *s3, char *s4)
{
	ft_putstr_fd("minishell: ", 2);
	if (s1)
		ft_putstr_fd(s1, 2);
	if (s2)
		ft_putstr_fd(s2, 2);
	if (s3)
		ft_putstr_fd(s3, 2);
	if (s4)
		ft_putstr_fd(s4, 2);
	ft_putstr_fd("\n", 2);
}

int	ft_isspacee(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return (0);
	return (1);
}


void	env_add_back(t_env **env, char *key, char *value, int hidden)
{
	t_env	*node;
	t_env	*tmp;

	node = create_env_node(key, value, hidden);
	tmp = *env;
	if (!node)
		return ;
	if (!*env)
	{
		*env = node;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}


void	update_double_quotes_case(t_tools *tools, t_lexer *current)
{
	char	*str;

	str = string_converter(current, tools);
	if (current->str)
		free(current->str);
	current->str = ft_strdup(str);
	free(str);
}

void	insert_at(t_lexer **lexer, t_lexer *current, char *str, int space)
{
	t_lexer	*tmp;
	t_lexer	*new;

	if (!lexer || !*lexer || !current)
		return ;
	tmp = *lexer;
	while (tmp != current)
		tmp = tmp->next;
	if (!tmp)
		return ;
	new = create_new_lexer_node(str, TOKEN_WORD, NO_QUOTE, space);
	new->next = tmp->next;
	tmp->next = new;
}


void	update_no_quotes_case(t_tools *tools, t_lexer **current, t_lexer **tmp)
{
	char	*str;
	char	**split;
	int		i;
	t_lexer	*to_del;

	str = string_converter(*tmp, tools);
	split = ft_split(str, " ");
	to_del = *tmp;
	(*tmp) = (*tmp)->next;
	if (!split || !*split)
		return (handle_empty_split(current, to_del, &split, &str), (void)0);
	i = 0;
	while (split[i + 1])
		i++;
	if (i == 0)
		return (handle_simple_string(to_del, &split, &str), (void)0);
	while (i >= 0)
	{
		if (!split[i + 1])
			insert_at(current, to_del, split[i], to_del->space);
		else
			insert_at(current, to_del, split[i], 1);
		i--;
	}
	(free_lexer_node(current, to_del), free_split(split), free(str));
}

static void	handle_qst_str(t_tools *tools, int **i, char ***result,
    size_t	size)
{
	char	*sub;

	sub = ft_itoa(tools->last_exit_status);
	ft_strlcat(**result, sub, size);
	free(sub);
	(**i)++;
}

static void	handle_special_cases(t_lexer *current, int **i, char ***result,
    size_t size)
{
	char	*sub;
	int		j;

	if (current->str[**i] && ft_isdigit(current->str[**i]))
	{
		(**i)++;
		j = 0;
		while (current->str[**i + j] && ft_isalnum(current->str[**i + j]))
			j++;
		sub = ft_substr(current->str, **i, j);
		if (sub)
			ft_strlcat(**result, sub, size);
		free(sub);
		**i += j;
		return ;
	}
	if (current->str[**i] && current->str[**i] == '$')
	{
		sub = ft_itoa((int)get_pid());
		ft_strlcat(**result, sub, size);
		free(sub);
		(**i)++;
		return ;
	}
}

static void	handle_empty_sub(char **sub, t_lexer *current, char ***result,
    size_t size)
{
	if (current->space || current->quote_type == DOUBLE_QUOTES)
	ft_strlcat(**result, "$", size);
	free(*sub);
}

void	handle_dollar_sign_str(t_lexer *current, t_tools *tools,
	int *i, char **result)
{
	int		j;
	char	*sub;
	t_env	*env_var;
	size_t	size;

	size = get_new_string_length(current, tools) + 1;
	(*i)++;
	j = 0;
	if (current->str[*i] && current->str[*i] == '?')
	return (handle_qst_str(tools, &i, &result, size), (void)0);
	if (is_a_special_case(current, i))
	return (handle_special_cases(current, &i, &result, size), (void)0);
	while (current->str[*i + j] && (ft_isalnum(current->str[*i + j])
		|| current->str[*i + j] == '_'))
	j++;
	sub = ft_substr(current->str, *i, j);
	if (!sub || !*sub)
	return (handle_empty_sub(&sub, current, &result, size), (void)0);
	env_var = get_env_var(tools->env, sub);
	if (env_var && env_var->value)
	ft_strlcat(*result, env_var->value, size);
	free(sub);
	*i += j;
}

char	*string_converter(t_lexer *current, t_tools *tools)
{
	char	*result;
	int		i;
	int		j;
	size_t	size;

	/* Make sure get_new_string_length accounts for removed escape backslashes */
	size = get_new_string_length(current, tools) + 1;
	result = ft_calloc(size, 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (current->str[i])
	{
		/* If in double quotes, check for backslash escapes */
		if (current->quote_type == DOUBLE_QUOTES && current->str[i] == '\\')
		{
			/* Only escape the characters that should be escaped */
			if (current->str[i + 1] && (current->str[i + 1] == '"' ||
				current->str[i + 1] == '$' ||
				current->str[i + 1] == '\\'))
			{
				result[j++] = current->str[i + 1];
				i += 2;
				continue;
			}
			/* If not a recognized escape, copy the backslash as is */
			else
			{
				result[j++] = current->str[i++];
				continue;
			}
		}
		if (current->str[i] == '$')
		{
			handle_dollar_sign_str(current, tools, &i, &result);
			j = ft_strlen(result); /* Update j to current length */
			continue;
		}
		result[j++] = current->str[i++];
	}
	return (result);
}


void	handle_empty_split(t_lexer **current, t_lexer *to_del,
    char ***split, char **str)
{
	free_lexer_node(current, to_del);
	if (*split && !(**split))
	free_split(*split);
	free(*str);
}

void	handle_simple_string(t_lexer *to_del, char ***split, char **str)
{
	if (to_del->str)
		free(to_del->str);
	to_del->str = ft_strdup(*str);
	free_split(*split);
	free(*str);
}

pid_t	get_pid(void)
{
	int		fd;
	char	buffer[256];
	pid_t	pid;
	ssize_t	bytes_read;
	char	*ptr;

	pid = -1;
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (pid);
	}
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	if (bytes_read == -1)
	{
		perror("read");
		close(fd);
		return (pid);
	}
	buffer[bytes_read] = '\0';
	fill_ptr_substr(&ptr, buffer, &pid);
	close(fd);
	return (pid);
}

int	get_new_string_length(t_lexer *current, t_tools *tools)
{
	int		len;
	int		i;

	if (!current->str || !current->str[0])
		return (0);
	len = 0;
	i = 0;
	while (current->str[i])
	{
		if (current->str[i] != '$')
		{
			len++;
			i++;
		}
		else
			handle_dollar_sign_len(current, tools, &i, &len);
	}
	return (len);
}

void	handle_question_mark(t_tools *tools, char **sub, int **i, int **len)
{
	*sub = ft_itoa(tools->last_exit_status);
	**len += ft_strlen(*sub);
	free(*sub);
	(**i)++;
}

void	handle_dollar_sign_len(t_lexer *current, t_tools *tools,
		int *i, int *len)
{
	int		j;
	char	*sub;
	t_env	*env_var;

	(*i)++;
	if (current->str[*i] && current->str[*i] == '?')
		return (handle_question_mark(tools, &sub, &i, &len), (void)0);
	if (is_a_special_case(current, i))
		return (handle_dollar_special_case(current, &sub, &len, &i), (void)0);
	j = 0;
	while (current->str[*i + j] && (ft_isalnum(current->str[*i + j])
			|| current->str[*i + j] == '_'))
		j++;
	sub = ft_substr(current->str, *i, j);
	if (!sub || !*sub)
	{
		if (current->space || current->quote_type == DOUBLE_QUOTES)
			*len += 1;
		free(sub);
		return ;
	}
	env_var = get_env_var(tools->env, sub);
	if (env_var)
		*len += ft_strlen(env_var->value);
	(free(sub), *i += j);
}

int	is_a_special_case(t_lexer *current, int *i)
{
	return ((current->str[*i] && ft_isdigit(current->str[*i]))
		|| (current->str[*i] && current->str[*i] == '$'));
}

void	fill_ptr_substr(char **ptr, char *buffer, pid_t *pid)
{
	*ptr = buffer;
	while (**ptr != ' ' && **ptr != '\0')
		(*ptr)++;
	**ptr = '\0';
	*pid = ft_atoi(buffer);
}

void	handle_dollar_special_case(t_lexer	*current, char **sub,
    int **len, int **i)
{
	int	j;

	if (current->str[**i] && ft_isdigit(current->str[**i]))
	{
		(**i)++;
		j = 0;
		while (current->str[**i + j] && ft_isalnum(current->str[**i + j]))
		j++;
		**len += j;
		return ;
	}
	if (current->str[**i] && current->str[**i] == '$')
	{
		*sub = ft_itoa((int)get_pid());
		**len += ft_strlen(*sub);
		free(*sub);
		(**i)++;
		return ;
	}
}

int	open_file(char *filename, int oflags, int access)
{
	int	fd;

	if (access == 0)
		fd = open(filename, oflags);
	else
		fd = open(filename, oflags, access);
	if (fd < 0)
	{
		print_message_error(filename, ": ", strerror(errno), 0);
		return (-1);
	}
	return (fd);
}
