NAME = minishell

SRC_LIST =  build_tree_utils.c \
			build_tree.c \
			cd_buildin.c \
			check_heredocs.c \
			constructors.c \
			echo_env_pwd_buildin.c \
			errors.c \
			execute_node_exec.c \
			execute_node_pipe.c \
			execute_tree.c \
			exit_buildin.c \
			expand_token_var.c \
			export_buildin.c \
			export_print.c \
			fill_token_list.c \
			free_data.c \
			free_tree.c \
			handle_tokens.c \
			minishell_utils.c \
			minishell.c \
			signals1.c \
			signals2.c \
			token_list.c \
			unset_buildin.c \
			utils_init.c \
			var_utils.c \

SRC_DIR = src/
SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))

BIN_DIR = bin/
BIN = $(addprefix $(BIN_DIR), $(SRC_LIST:.c=.o))

LIBFT_DIR = libft/
LIBFT = $(addprefix $(LIBFT_DIR), libft.a)

CC = cc
CFLAGS = -Wall -Werror -Wextra #-g -fsanitize=address 
RM = rm -rf

all: $(NAME)

$(NAME): $(BIN) $(LIBFT)
	$(CC) $(CFLAGS) $(BIN) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

$(BIN_DIR)%.o: $(SRC_DIR)%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	$(RM) $(BIN_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

norm:
	norminette -R CheckForbiddenSourceHeader include/
	norminette $(SRC_DIR)
	norminette $(LIBFT_DIR)
