NAME = minishell
NAME_BONUS = minishell_bonus

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
			signals.c \
			token_list.c \
			unset_buildin.c \
			utils_init.c \
			var_utils.c \

SRC_LIST_BONUS = minishell.c

SRC_DIR = src/
SRC_DIR_BONUS = src_bonus/
SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))
SRC_BONUS = $(addprefix $(SRC_DIR_BONUS), $(SRC_LIST_BONUS))

BIN_DIR = bin/
BIN_DIR_BONUS = bin_bonus/
BIN = $(addprefix $(BIN_DIR), $(SRC_LIST:.c=.o))
BIN_BONUS = $(addprefix $(BIN_DIR_BONUS), $(SRC_LIST_BONUS:.c=.o))

LIBFT_DIR = libft/
LIBFT = $(addprefix $(LIBFT_DIR), libft.a)

CC = cc
CFLAGS = -g -fsanitize=address -Wall -Werror -Wextra
RM = rm -rf

all: $(NAME)

$(NAME): $(BIN) $(LIBFT)
	$(CC) $(CFLAGS) $(BIN) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

$(BIN_DIR)%.o: $(SRC_DIR)%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir $(BIN_DIR)

bonus: $(BIN_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) $(BIN_BONUS) -L$(LIBFT_DIR) -lft -o $(NAME)

$(BIN_DIR_BONUS)%.o: $(SRC_DIR_BONUS)%.c | $(BIN_DIR_BONUS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR_BONUS):
	mkdir $(BIN_DIR_BONUS)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	$(RM) $(BIN_DIR)
	$(RM) $(BIN_DIR_BONUS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(NAME_BONUS)
	make fclean -C $(LIBFT_DIR)

re: fclean all

norm:
	norminette -R CheckForbiddenSourceHeader include/
	norminette $(SRC_DIR)
