# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/31 16:49:23 by dehamad           #+#    #+#              #
#    Updated: 2024/06/10 20:42:39 by dehamad          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g 
PTHREAD_FLAGS = -lpthread

SRC = main.c parsing.c data_utils.c philo_utils.c utils.c
OBJ = $(SRC:.c=.o)

ARGS = 4 410 200 200 

all: $(NAME) clean
	./$(NAME) $(ARGS)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(PTHREAD_FLAGS)

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(OBJ) $(NAME)

re: fclean all

sanitize: CFLAGS += -g3 -fsanitize=address
sanitize: re

valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(ARGS)

helgrind:
	valgrind -s --tool=helgrind ./$(NAME) $(ARGS)

.PHONY: all clean fclean re sanitize valgrind helgrind
