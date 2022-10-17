SRCS	=	main.c ft_atoi.c init.c start_routine.c routine.c check_deaths.c

HEADER_FILES = philo.h

OBJS	=	${SRCS:.c=.o}

CC		=	gcc
RM		=	rm -f

CFLAGS	=	-Wall -Wextra -Werror -pthread -fsanitize=thread

all:		philo

%.o : %.c 	${HEADER_FILES}
		${CC} -c ${CFLAGS} $< -o ${<:.c=.o}

philo:	${OBJS}
			@${CC} ${CFLAGS} ${OBJS} -o philo

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} philo

re:			fclean all

.PHONY:		all clean fclean re philo