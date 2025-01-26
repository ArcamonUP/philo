# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/07 11:00:38 by kbaridon          #+#    #+#              #
#    Updated: 2025/01/26 15:26:37 by kbaridon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(INCDIR)
NAME = philo_bonus
SRCDIR = src
INCDIR = include
HEADERS = $(INCDIR)/philo_bonus.h

SRC =	philo_bonus.c	\
		init_bonus.c	\
		threads_bonus.c	\
		moves_bonus.c	\
		utils_bonus.c	\
		free_bonus.c

OBJS = $(addprefix $(SRCDIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS): $(HEADERS)

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re