# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 12:50:00 by ksinn             #+#    #+#              #
#    Updated: 2025/03/17 12:44:28 by ksinn            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test_tokenizer
LEAK_TEST = test_memory_leaks

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

COMMON_SRCS = ../src/Tokenizer/tokenize.c \
              ../src/Tokenizer/ms_split.c \
              ../src/Tokenizer/ms_split_helper.c \
              ../src/Tokenizer/ft_strndup.c \
              ../src/GarbageCollector/gc_malloc.c \
              ../src/GarbageCollector/gc_holder.c \
              ../src/GarbageCollector/gc_free_context.c

SRCS = test_tokenizer_gc.c $(COMMON_SRCS)
LEAK_SRCS = test_memory_leaks.c $(COMMON_SRCS)

INCLUDES = -I../includes -I../libft/includes

LIBFT = ../libft/libft.a

all: $(NAME) $(LEAK_TEST)

$(NAME): $(SRCS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) $(LIBFT) -o $(NAME)

$(LEAK_TEST): $(LEAK_SRCS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(LEAK_SRCS) $(LIBFT) -o $(LEAK_TEST)

$(LIBFT):
	make -C ../libft

clean:
	make -C ../libft clean

fclean: clean
	rm -f $(NAME) $(LEAK_TEST)
	make -C ../libft fclean

re: fclean all

.PHONY: all clean fclean re
