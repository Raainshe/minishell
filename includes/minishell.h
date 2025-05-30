/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:18:57 by ksinn             #+#    #+#             */
/*   Updated: 2025/05/13 13:30:19 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "garbage_collector.h"
# include "libft.h"
# include "typedef.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# define PATH_MAX 4096

/* ms_split_helper_helper_helper.c */
int								ft_isspace(char c);
bool							ft_is_operator(char c);
bool							ft_is_multi_char_op(char *str, int i);
/* ms_split_helper_helper.c */
bool							ft_handle_token_start(t_token_info *info);
bool							ft_handle_token_end(t_token_info *info);
bool							ft_handle_operator(t_token_info *info,
									char current, char next);
bool							ft_handle_next_operator(t_token_info *info,
									char next);
bool							ft_handle_multi_char_op(t_token_info *info,
									char current, char next);
/* ms_split_helper.c */
void							ft_update_quote_state(char c, bool *in_quote,
									bool *in_double_quote);
int								ft_count_tokens(char *str);
void							ft_init_token_info(t_token_info *info,
									char *str, int token_count);
/* ms_split.c */
char							**ft_split_tokens(char *str);
bool							ft_add_token(t_token_info *info, int start,
									int end);
/* ft_strndup.c */
char							*ft_strndup(const char *s, size_t n);
/* tokenize.c */
void							ft_free_tokens(char **tokens, int j);
t_token							*ft_tokenize(char **tokens, t_list *env);
bool							has_quotes(char *str);
/* parser_utils.c */
void							init_parser_context(t_parser_context *context,
									t_token *tokens);
t_token							current_token(t_parser_context *context);
t_token							peek_next_token(t_parser_context *context);
t_token							next_token(t_parser_context *context);
void							parser_error(t_parser_context *context,
									char *msg);
/* nodes.c */
t_node							*create_node(t_node_type type, void *data,
									t_node *left, t_node *right);
t_node							*create_command_node(char **args);
t_node							*create_redirect_node(t_node_type type,
									char *filename, t_node *command);
t_node							*create_pipe_node(t_node *left, t_node *right);
/* parser.c */
t_node							*parse_tokens(t_token *tokens);
t_node							*parse_pipeline(t_parser_context *context);
/*parser_command.c*/
t_node							*parse_command(t_parser_context *context);
/*parser_redirect.c*/
t_node							*handle_redirection(t_parser_context *context,
									t_node *command);
/*executor.c*/
int								execute_node(t_node *node, t_list **env);
/*execute_command.c*/
int								execute_command(t_node *node, t_list **env);
/*execute_command_helper.c*/
void							print_builtin_error(char *str);
char							*find_command_path(char *cmd, t_list *env);
/*execute_pipe.c*/
int								execute_pipe(t_node *node, t_list **env);
/*execute_redirect.c*/
int								execute_redirect(t_node *node, t_list **env);
/*execute_redirect_helper.c*/
int								handle_heredoc_parent(int pipe_fd[2],
									pid_t pid);
int								setup_heredoc_terminal(void);
int								redirect_stdin_to_tty(int tty_fd);
void							restore_stdin(int saved_stdin);
void							process_heredoc_line(t_proc_heredoc_line *phl);
/*execute_redirect_helper_helper.c*/
void							clean_and_exit(int pipe_fd, int tty_fd,
									int exit_code);
int								setup_heredoc_signal(struct sigaction
									*sa_orig_quit,
									struct sigaction *sa_ign_quit,
									int pipe_fd[2]);
int								wait_for_heredoc_child(pid_t pid, int *status,
									int pipe_fd,
									struct sigaction *sa_orig_quit);
void							restore_heredoc_signal(struct sigaction
									*sa_orig_quit);
int								process_heredoc_status(int status, int pipe_fd);
/*execute_redirect_helper_helper_helper.c*/
int								print_redirect_error(t_redirect *redirect);
void							handle_heredoc_child(int pipe_fd[2],
									char *delimiter, t_list *env,
									bool expand_vars);
/*redirect_in.c*/
int								handle_redirect_in(t_node *node, t_list **env);
/*redirect_out.c*/
int								handle_redirect_out(t_node *node, t_list **env);
/*./Execute/builtins/*/
int								builtin_echo(char **args);
int								builtin_cd(char **args, t_list **env);
int								builtin_pwd(char **args);
int								builtin_unset(char **args, t_list **env);
int								builtin_env(t_list *env);
int								builtin_exit(char **args);
/*builtin_export.c*/
int								builtin_export(char **args, t_list **env);
char							*prepare_export_arg(char *orig_arg);
char							*create_appended_value(char *existing,
									char *new_value);
bool							has_append_operator(char *str);
/*builtin_export_helper.c*/
int								print_error(char *arg);
void							add_arg(t_list **env, char *arg);
int								process_export_arg(char *arg, t_list **env);
/*builtin_export_helper_helper.c*/
bool							is_valid(char *str);
int								var_len(char *arg);
t_list							*find_duplicate(char *arg, t_list **env);
/* heredoc.c */
int								handle_here_doc(t_node *node, t_list **env);
/* heredoc_helper.c */
int								preprocess_heredocs(t_node *node, t_list **env);
void							close_heredoc_fds(int count);
int								handle_heredoc(char *delimiter, t_list *env,
									bool expand_vars);
int								process_heredoc_node(t_node *node,
									t_list **env);
int								traverse_tree_nodes(t_node *node, t_list **env);
/* heredoc_helper_helper.c */
void							init_heredoc_fds(t_node *node);
void							close_preprocessed_heredocs(t_node *node);
t_node							*collect_heredoc_nodes(t_node *node,
									t_node *heredoc_nodes[100],
									int *heredoc_count);
int								handle_heredoc_error(int last_fd);
/* environ.c */
t_list							*copy_environ(char **environ);
char							**convert_env_to_array(t_list *env);
char							*ft_getenv(const char *name, t_list *env);
/* expansion.c */
char							*expand(char *str, t_list *env);
char							*ft_strjoin_char(char *str, char c);
char							*expand_variables(char *str, t_list *env);
/* expansion_helper.c */
char							*process_exit_code(char *result, int *i);
char							*get_env_value(char *var_name, t_list *env);
char							*extract_var_name(char *str);
char							*process_variable(char *str, t_list *env,
									char *result, int *i_ptr);
char							*process_tilde_expansion(char *str, t_list *env,
									char *result, int *i_ptr);
/*expansion_utlis.c*/
void							initialise_expand_var(int *i, int *in_quotes,
									char *quote_type);
void							update_quote_type(int *in_quotes,
									char *quote_type, char character, int *i);
void							update_quote_type_neg(int *in_quotes,
									char *quote_type, int *i);
char							*process_expansion_loop(char *str, t_list *env,
									char *result);
char							*process_var_expansion(char *str, t_list *env,
									char *result, int *i);
char							*append_char_to_result(char *result, char *str,
									int *i);
/*expansion_utlis_two.c*/
bool							is_expandable_variable(char *str, int i,
									char quote_type, int in_quotes);
void							init_expansion_info(t_expansion_info *info,
									char *result);
char							*handle_initial_tilde(char *str, t_list *env,
									t_expansion_info *info);
char							*handle_quotes(char *str,
									t_expansion_info *info);
char							*process_expansion_char(char *str, t_list *env,
									t_expansion_info *info);

/* signals.c */
void							setup_interactive_signals(void);
void							setup_noninteractive_signals(void);
void							setup_heredoc_signals(void);
/*signals_helper.c */
void							handle_sigint_heredoc(int signum);
void							handle_sigquit_noninteractive(int signum);
void							reset_signals(void);
void							reset_term_after_signal(void);
int								get_signal_status(void);
/* exit.c */
int								*ft_exit_code_holder(void);
/* minishell_utils.c */
void							print_banner(void);
bool							is_only_spaces(char *str);
void							cleanup_execution_resources(t_node *ast);
t_list							*init_shell(int **exit_code);
t_node							*parse_input(char *input, t_list *env);

extern char						**environ;
extern volatile sig_atomic_t	g_signal_received;

#endif
