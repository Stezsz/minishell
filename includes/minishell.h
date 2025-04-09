/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:20:02 by strodrig          #+#    #+#             */
/*   Updated: 2025/01/27 12:20:02 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <fcntl.h>
# include <signal.h>

# define EQUAL 0

# define RESET_COLOR "\x1B[0m"
# define CYAN "\x1B[36m"
# define MAGENTA "\x1B[35m"

typedef struct sigaction	t_sigaction;

typedef enum e_bool
{
	FALSE,
	TRUE,
}	t_bool;

enum e_token
{
	WORD = -1,
	WHITE_SPACE = ' ',
	NEWLINE_TOKEN = '\n',
	QOUTES = '\'',
	DQUOTES = '\"',
	ESCAPE = '\\',
	ENV = '$',
	PIPE_TOKEN = '|',
	REDIR_IN = '<',
	REDIR_OUT = '>',
	HERE_DOC,
	DREDIR_OUT,
	_NULL = 0,
};

enum e_shell_state
{
	SH_READING,
	SH_EXECUTING,
};

typedef struct s_elem		t_elem;
typedef struct s_list		t_list;
typedef struct s_data		t_data;
typedef struct s_cmd		t_cmd;
typedef struct s_pipe		t_pipe;
typedef struct s_ast_node	t_ast_node;
typedef struct s_ast		t_ast;
typedef struct s_env_elem	t_env_elem;
typedef struct s_env		t_env;
typedef union u_union		t_union;

enum e_node_type
{
	CMD,
	PIPE,
};

typedef struct s_env_elem
{
	char				*key;
	char				*value;
	struct s_env_elem	*next;
	struct s_env_elem	*prev;
}	t_env_elem;

typedef struct s_env
{
	t_env_elem			*head;
	char				**env;
	int					size;
}	t_env;

enum e_state
{
	IN_DQUOTE,
	IN_QUOTE,
	ESCAPED,
	GENERAL,
};

typedef struct s_cmd
{
	char	**args;
	char	**env;
}	t_cmd;

typedef struct s_pipe
{
	t_ast_node	*left;
	t_ast_node	*right;
}	t_pipe;

typedef union u_union
{
	t_pipe		*pipe;
	t_cmd		*cmd;
}	t_union;

typedef struct s_ast_node
{
	enum e_node_type	type;
	t_union				*content;
}	t_ast_node;

typedef struct s_ast
{
	t_ast_node	*root;
}	t_ast;

typedef struct s_elem
{
	char			*data;
	int				len;
	enum e_token	type;
	enum e_state	state;
	t_elem			*next;
	t_elem			*prev;
}	t_elem;

typedef struct s_list
{
	t_elem	*head;
	t_elem	*tail;
	int		size;
}	t_list;

typedef struct s_data
{
	int					exit_status;
	int					pid;
	int					which;
	int					alloc;
	char				**env;
	char				*siglist[NSIG];
	t_list				*lex;
	t_env				*ev;
	t_ast				*tree;
	enum e_shell_state	shell_state;
}	t_data;

/* --- minishell.c --- */
void	init_data(t_data *data);

/* --- utils.c --- */
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strlen(const char *s);
int		ft_strisspace(char *str);
int		has_unclosed_quotes(const char *input);
char	*handle_unclosed_quotes(char *input);

/* --- signals.c --- */
void	init_signals(t_data *data);
void	signals_handler(int sig, siginfo_t *siginfo, void *ptr);
void	catch_sig(t_sigaction *sa);
void	mask_signals(t_sigaction *sa);
void	init_sig_list(t_data *data);

/* --- program.c --- */
void	write_prompt(void);
int		ft_readline(char **line);
void	set_g_data(char **env);
void	launch_shell(char **env);
void	execute_piped(char **cmds[], int num_cmds, char **envp);

/* --- exec.c --- */
char	*find_executable(char *cmd);
void	handle_redirections(char **args);
void	execute_command(char **args, char **envp);
void	execute_piped_commands(char **cmd1, char **cmd2, char **envp);

/* --- builtins/echo.c --- */
void	builtin_echo(char **args);

/* --- builtins/cd.c --- */
void	builtin_cd(char **args);

/* --- builtins/pwd.c --- */
void	builtin_pwd(void);

/* --- builtins/export.c --- */
void	builtin_export(char **args);

/* --- builtins/unset.c --- */
void	builtin_unset(char **args);

/* --- builtins/env.c --- */
void	builtin_env(char **envp);

/* --- builtins/exit.c --- */
void	builtin_exit(char **args);

/* --- builtins.c --- */
int		is_builtin(char *cmd);
void	execute_builtin(char **args, char **envp);

/* --- parser/ast.c --- */
t_ast	*init_tree(t_ast *tree);

/* --- env_var.c --- */
char	*expand_variable(char *arg);
void	expand_args(char **args);

extern t_data	*g_data_ptr;

#endif
