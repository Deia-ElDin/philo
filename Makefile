# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/31 16:49:23 by dehamad           #+#    #+#              #
#    Updated: 2024/06/02 01:51:09 by dehamad          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c data_utils.c philo_utils.c utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME) clean
	./philo 4 450 200 200 
sanitize: CFLAGS += -g3 -fsanitize=address
sanitize: re

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(OBJ) $(NAME)

re: fclean all

.PHONY: all clean fclean re sanitize
