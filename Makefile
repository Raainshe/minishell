NAME = minishell

SRC_DIR = src
OUT_DIR = out

# Object Files
OBJS = $(addprefix $(OUT_DIR)/, $(SRCS:.c=.o))

SRC_SUBDIRS =	$(SRC_DIR) \
				$(SRC_DIR)/Tokenizer \
				$(SRC_DIR)/GarbageCollector \
				$(SRC_DIR)/AST \
				$(SRC_DIR)/Execute \
				$(SRC_DIR)/Execute/builtins \
				$(SRC_DIR)/Signals \
				$(SRC_DIR)/Exit

VPATH = $(SRC_SUBDIRS)

# source files
SRCS =	minishell.c \
		minishell_utils.c \
		ms_split.c \
		ms_split_helper.c \
		ms_split_helper_helper.c \
		ms_split_helper_helper_helper.c \
		expansion.c \
		expansion_helper.c \
		expansion_utils.c \
		expansion_utlis_two.c \
		ft_strndup.c \
		tokenize.c \
		gc_free_context.c \
		gc_malloc.c \
		gc_holder.c \
		nodes.c \
		parser_command.c \
		parser_redirect.c \
		parser_utils.c \
		parser.c \
		executor.c \
		execute_command.c \
		execute_command_helper.c \
		execute_pipe.c \
		execute_redirect.c \
		execute_redirect_helper.c \
		execute_redirect_helper_helper.c \
		execute_redirect_helper_helper_helper.c \
		redirect_in.c \
		redirect_out.c \
		heredoc.c \
		heredoc_helper.c \
		heredoc_helper_helper.c \
		builtin_pwd.c \
		builtin_echo.c \
		builtin_exit.c \
		builtin_cd.c \
		builtin_env.c \
		builtin_export.c \
		builtin_export_helper.c \
		builtin_export_helper_helper.c \
		builtin_unset.c \
		environ.c \
		signals.c \
		signals_helper.c \
		exit.c

# Compiler
CC = cc
CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -g -fsanitize=address -fsanitize=undefined
INCLUDES = -I./includes -I./libft/includes
LDFLAGS = -lreadline

# Libft
LIBFT = libft/libft.a
LIBFT_DIR = libft

all: $(NAME)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: LDFLAGS += $(DEBUG_FLAGS)
debug: $(NAME)
	@echo "$(GREEN)$(NAME) built with debug flags!$(NC)"

$(OUT_DIR):
	@mkdir -p $(OUT_DIR)
	@echo "$(BLUE)Created $(OUT_DIR) directory$(NC)"

$(LIBFT):
	@echo "$(BLUE)Compiling libft...$(NC)"
	@make -C $(LIBFT_DIR)

$(NAME): $(OUT_DIR) $(LIBFT) $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) built successfully!$(NC)"
	@echo "$(GREEN)Run with ./minishell :)$(NC)"

$(OUT_DIR)/%.o: %.c
	@echo "$(CYAN)Compiling $<...$(NC)"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(PURPLE)Cleaning object files...$(NC)"
	@rm -rf $(OUT_DIR)
	@make clean -C $(LIBFT_DIR)
	@echo "$(GREEN)Clean completed!$(NC)"

fclean: clean
	@echo "$(PURPLE)Removing $(NAME)...$(NC)"
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@echo "$(GREEN)Full clean completed!$(NC)"

re: fclean all

.PHONY: all clean fclean re debug
# Colours
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
NC = \033[0m
