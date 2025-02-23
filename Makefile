NAME = minishell

LIBFT = libft

LIBFT_LIB = libft/libft.a

SRCS =  minishell.c								\
		art.c									\
		src/lexer/handle_lexer.c				\
		src/lexer/lexer_utils.c					\
		src/lexer/lexer_sublist.c				\
		src/lexer/parse_lexer.c					\
		src/lexer/utils_1.c						\
		src/parser/free_parser.c				\
		src/parser/parser_checker.c				\
		src/parser/utils.c 						\
		src/parser/parser_checker2.c			\
		src/parser/set_builtin.c				\
		src/parser/hd_limiter.c					\
		src/parser/tokens.c						\
		src/parser/tokens2.c					\
		src/parser/redirection.c				\
		src/utils/utils.c						\
		src/builtins/echo.c						\
		src/builtins/exit.c						\
		src/builtins/env.c						\
		src/builtins/export.c					\
		src/builtins/unset.c					\
		src/builtins/cd.c						\
		src/builtins/pwd.c						\
		src/builtins/utils.c					\
		src/builtins/utils2.c					\
		src/builtins/utils3.c					\
		src/execute_command/execute1.c			\
		src/execute_command/execute2.c			\
		src/execute_command/execute3.c			\
		src/execute_command/execute4.c			\
		src/execute_command/exe21.c				\
		src/execute_command/exe31.c				\
		src/execute_command/exe32.c				\
		src/execute_command/utils1.c			\
		src/execute_command/utils2.c			\
		src/execute_command/utils3.c			\
		src/signals/signals2.c					\
		src/signals/signals1.c					\
		src/heredoc/test.c						\
		src/tools/test.c 						\
		src/lexer/utils_2.c						\
		src/execute_command/herdoc_utils.c		\
		src/errors/errors.c						\
	
		
OBJDIR = objs
OBJCS = $(SRCS:%.c=$(OBJDIR)/%.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -g
READLINE_FLAG = -lreadline

all: $(NAME)

$(OBJDIR): 
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJCS)
	make -C $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJCS) $(READLINE_FLAG) $(LIBFT_LIB)
	@printf "Compiling the entire program...\n"
	@sleep 1
	@printf "\033[A\033[K"
	@printf "\033[32m### Minishell Compiled Successfully! ###\n\033[0m"

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT) clean
	rm -f $(OBJCS)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(LIBFT_LIB)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re