NAME = libft.a
SRC_DIR = src
OUT_DIR = out

VPATH = src/string:src/mem:src/put_utils:src/lst:src/printf:src/gnl

STRING_SRCS =		ft_atoi.c			\
					ft_isalnum.c		\
					ft_isalpha.c		\
					ft_isascii.c		\
					ft_isdigit.c		\
					ft_isprint.c		\
					ft_itoa.c			\
					ft_split.c			\
					ft_strchr.c			\
					ft_strdup.c			\
					ft_striteri.c		\
					ft_strjoin.c		\
					ft_strlen.c			\
					ft_strmapi.c		\
					ft_strncmp.c		\
					ft_strnstr.c		\
					ft_strrchr.c		\
					ft_strtrim.c		\
					ft_substr.c			\
					ft_tolower.c		\
					ft_toupper.c		\
					ft_strlower.c

MEM_SRCS =			ft_bzero.c			\
					ft_calloc.c			\
					ft_memchr.c			\
					ft_memcmp.c			\
					ft_memcpy.c			\
					ft_memmove.c		\
					ft_memset.c			\
					ft_strlcat.c		\
					ft_strlcpy.c

PUT_UTILS_SRCS =	ft_putchar_fd.c		\
					ft_putendl_fd.c		\
					ft_putnbr_fd.c		\
					ft_putstr_fd.c

LST_SRCS =			ft_lstadd_back.c 	\
					ft_lstadd_front.c	\
					ft_lstclear.c		\
					ft_lstdelone.c		\
					ft_lstiter.c		\
					ft_lstlast.c		\
					ft_lstmap.c			\
					ft_lstnew.c			\
					ft_lstsize.c

PRINTF_SRCS = 		ft_printf.c			\
					ft_print_p.c		\
					ft_print_s.c		\
					ft_print_u.c		\
					ft_print_x.c

GNL_SRCS =	get_next_line.c				\
			get_next_line_utils.c

SRCS = $(STRING_SRCS) $(MEM_SRCS) $(PUT_UTILS_SRCS) $(LST_SRCS) $(PRINTF_SRCS) $(GNL_SRCS)

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -I../includes

OBJS = $(addprefix $(OUT_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OUT_DIR) $(OBJS)
	ar -rcs $(NAME) $(OBJS)

$(OUT_DIR):
	mkdir -p $@

$(OUT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUT_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
