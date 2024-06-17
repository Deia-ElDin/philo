# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/31 16:49:23 by dehamad           #+#    #+#              #
#    Updated: 2024/06/17 18:47:49 by dehamad          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g 
PTHREAD_FLAGS = -lpthread

SRC = main.c parsing.c data_utils.c philo_utils.c utils.c
OBJ = $(SRC:.c=.o)

ARGS = 2 410 200 200

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(PTHREAD_FLAGS)

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(OBJ) $(NAME)

re: fclean all

sanitize: CFLAGS += -g3 -fsanitize=address
sanitize: re

start: re clean
	./$(NAME) $(ARGS)
	
valgrind: re clean
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(ARGS)
	
helgrind: re clean
	valgrind -s --tool=helgrind --tool=drd ./$(NAME) $(ARGS)

.PHONY: all clean fclean re sanitize start valgrind helgrind
