/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:55:43 by fernacar          #+#    #+#             */
/*   Updated: 2023/11/20 21:24:41 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# include <readline/readline.h>
# include <readline/history.h>

# include <signal.h>

# include <sys/types.h>
# include <sys/wait.h>

# include <linux/limits.h>
# include <limits.h>

# include <errno.h>

# define PROMPT "\001\e[1;37m\002minis\001\e[31;5m\002hell\001\e[0m\002> "

# define EXEC		1
# define REDIR		2
# define PIPE		3
# define HEREDOC	4

extern int	g_exit;

typedef struct s_tnode{
	int				type;
}			t_tnode;

typedef struct s_tnode_pipe{
	int				type;
	t_tnode			*branch_left;
	t_tnode			*branch_right;
}			t_tnode_pipe;

typedef struct s_tnode_exec{
	int		type;
	char	**argv;
	char	**eargv;
}			t_tnode_exec;

typedef struct s_tnode_redir{
	int		type;
	char	*file;
	int		flags;
	int		fd;
	t_tnode	*node;
}			t_tnode_redir;

typedef struct s_tnode_heredoc{
	int		type;
	char	*delm;
	char	*input;
	t_tnode	*node;
}			t_tnode_heredoc;

typedef struct s_token{
	char	*token;
	int		is_operator;
}			t_token;

typedef struct s_minishell
{
	char	**env;
	char	**uninit;
	char	**token_list;
	t_tnode	*tree_root;
	int		*exit_status;
}			t_minishell;

// signals
void	wait_signal_main(void);
void	wait_signal_child(void);
void	wait_signal_heredoc(void);
void	main_wait_signals(void);

// constructors
t_tnode	*construct_exec(void);
t_tnode	*construct_redir(t_tnode *subnode, char *file, int flags, int fd);
t_tnode	*construct_pipe(t_tnode *left, t_tnode *right);
t_tnode	*construct_heredoc(t_tnode *subnode, char *delm);

// free_data
void	free_commands(t_minishell *data);
void	free_data(t_minishell *data);

// minishell_utils
void	panic(char *str);
int		fork1(void);
char	*my_getenv(char *var, char **env);
char	get_token_type(char *token);
char	*strjoin_free(char *s1, char *s2);

// build_tree_utils
void	get_redir_node(t_tnode **node, char ***tokens,
			char token_type, int *invalid_syntax);
void	insert_redir_node(char ***tokens, t_tnode **ret_node,
			t_tnode *exec_node, int *invalid_syntax);

// build_tree
t_tnode	*parse_exec(char ***tokens, int *invalid_syntax);
t_tnode	*parse_redir(t_tnode *node, char ***tokens, int *invalid_syntax);
t_tnode	*parse_pipe(char ***tokens, int *invalid_syntax);
t_tnode	*build_tree(char **tokens, int *exit_status);

// check_heredocs
void	check_heredocs(t_tnode *node, t_minishell *data);

// execute_node_exec
void	execute_node_exec(t_tnode_exec *exec_node, t_minishell *data);

// execute_node_pipe
void	execute_node_pipe(t_tnode_pipe *pipe_node, t_minishell *data);

// execute_node
void	execute_node(t_tnode *node, t_minishell *data);

// free_tree
void	free_node(t_tnode *node);

// fill_token_list
void	fill_token_list(char **token_list, char *input);

// expand_token_var
void	skip_var(char *token, int *i);
char	*get_var_val(char *token, int i, char **env, int exit_status);
int		do_single_quotes(char *token, char	**expanded_token, int *i);
void	handle_quote_type(char quote, char *quote_type);
char	*expand_token_var(char *token, char **env, int exit_status,
			char quote_type);

// handle_tokens
void	handle_tokens(char **token_list, char **env, int exit_status);

// token_list
int		is_delimiter(char c);
char	**make_token_list(char *input, char **env, int exit_status);

// exit
void	exit_buildin(char **argv, int *exit_status);

//	errors
int		ft_perror(char *str, char letter, int *exit_status);
int		perror_identifier(char *command, char *str, int *exit_status);
void	perror_option_identifier(char *command, char *arg, int *exit_status);
int		perror_cd(int *exit_status);

// cd
int		cd_buildin(char **argv, char ***env_cpy, int *exit_status);

//	echo_env_pwd
void	echo_buildin(char **argv, int *exit_status);
void	env_buildin(char **argv, char **env_copy, int *exit_status);
void	pwd_buildin(char **argv, int *exit_status);

//	export
char	**substitute_env_str(char *arg, char **new_copy, size_t env_index);
void	export_buildin(char **arg, char ***env_copy, char ***uninit,
			int *exit_status);

//	export_print
void	print_export(char **copy, char **uninitialized, int *exit_status);

//	unset
void	unset_buildin(char **arg, char ***copy, char ***uninit,
			int *exit_status);
void	remove_uninit(char *argv, char ***uninit);
void	remove_env(char *argv, char ***copy, int key_index);

//	utils_init
int		is_option(char *str);
int		is_option_n(char *str);
int		ft_argc(char **argv);
int		key_alnum_under(char *s, size_t index);
void	init_env(char **env);

//	var_utils
int		key_length(char *str);
size_t	pointer_array_len(char **pointers);
char	*extract_key(char *str);
size_t	find_key_env_index(char *arg, char **env, int index);
char	**env_copy(char **env);

#endif
