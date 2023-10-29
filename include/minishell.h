/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:55:43 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/29 13:57:21 by fernacar         ###   ########.fr       */
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

# include <linux/limits.h> // max args
# include <limits.h> // max args

# include <errno.h>

# define EXEC		1
# define REDIR		2
# define PIPE		3
# define HEREDOC	4

typedef struct s_tnode{
	int				type;
}			t_tnode;

typedef struct s_tnode_pipe{
	int				type;
	t_tnode *branch_left;
	t_tnode *branch_right;
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
	t_tnode	*node;
	int		is_done;
}			t_tnode_heredoc;

typedef struct s_token{
	char	*token;
	int		is_operator;
}			t_token;

t_tnode	*construct_exec(void);
t_tnode	*construct_redir(t_tnode *subnode, char *file, int flags, int fd);
t_tnode	*construct_pipe(t_tnode *left, t_tnode *right);
t_tnode	*construct_heredoc(t_tnode *subnode, char *delm);

t_tnode	*parse_redir(t_tnode *node, char ***tokens);
t_tnode	*parse_exec(char ***tokens);
t_tnode	*parse_pipe(char ***tokens);
char	get_token_type(char *token);
t_tnode *build_tree(char **tokens);

void	panic(char *str);
int		fork1();
void	handle_sigint(int signal);
void	wait_signal(void);
void	execute_node(t_tnode *node, char **envp);
void	free_node(t_tnode *node);
void	print_node(t_tnode *node);

char	**make_token_list(char *input);

char	**execute_builtin(char **argv, char **env_copy);
void	pwd_buildin(char **argv);
void	echo_buildin(char **argv);
void	env_buildin(char **argv, char **env_copy);
char	**unset_buildin(char **argv, char **copy);

int		ft_perror(char *str, char letter);
char	**env_copy(char **env);

void 	print_list(char **list);


#endif
