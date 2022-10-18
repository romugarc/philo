# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/17 16:13:51 by rgarcia           #+#    #+#              #
#    Updated: 2022/10/18 16:26:24 by rgarcia          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME	= philo

SRCS	=	main.c ft_atoi.c init.c start_routine.c routine.c check_deaths.c end_routine.c

HEADER_FILES = philo.h

OBJS	=	${SRCS:.c=.o}

CC		=	gcc
RM		=	rm -f

CFLAGS	=	-Wall -Wextra -Werror -pthread #-fsanitize=thread

all:		${NAME}

%.o : %.c 	${HEADER_FILES}
		${CC} -c ${CFLAGS} $< -o ${<:.c=.o}

$(NAME):	${OBJS}
				@${CC} ${CFLAGS} ${OBJS} -o philo

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} philo

re:			fclean all

.PHONY:		all clean fclean re philo
