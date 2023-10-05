/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:55:43 by fernacar          #+#    #+#             */
/*   Updated: 2023/10/05 22:08:23 by fernacar         ###   ########.fr       */
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
	char	*efile;
	int		flags;
	int		fd;
	t_tnode	*node;
}			t_tnode_redir;

typedef struct s_tnode_heredoc{
	int		type;
	char	*delm_start;
	char	*delm_end;
	t_tnode	*node;
}			t_tnode_heredoc;


t_tnode	*construct_exec(void);
t_tnode	*construct_redir(t_tnode *subnode, char *file, char *efile, int flags, int fd);
t_tnode	*construct_pipe(t_tnode *left, t_tnode *right);
t_tnode	*construct_heredoc(t_tnode *subnode, char *delm_start, char *delm_end);

int		peek(char **token_start, char *token_end, char *tokens);
char	get_token(char **token_start, char *token_end, char **q, char **eq);
t_tnode	*nulterminate(t_tnode *node);

t_tnode	*parse_redir(t_tnode *node, char **token_start, char *token_end);
t_tnode	*parse_exec(char **token_start, char *token_end);
t_tnode	*parse_pipe(char **token_start, char *token_end);
t_tnode *build_tree(char *input);

void	panic(char *str);
int		fork1();
void	handle_sigint(int signal);
void	wait_signal(void);
void	execute_node(t_tnode *node);


char	**make_token_list(char *input);
void 	print_list(char **list);

#endif
