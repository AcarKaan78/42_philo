SRCS = 		./philosophers.c ./routine.c ./utils_control.c \
		 	./utils_lock_time.c utils_numbers.c utils_philosophers.c

OBJS = $(SRCS:.c=.o)

NAME = philo
CC = gcc
CFLAGS =  -g -fsanitize=thread -Wall -Wextra -Werror
RM = rm -rf

all: ${NAME}

$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@${RM} ${OBJS}

fclean:
	@${RM} $(NAME)
	@${RM} ${OBJS}

re: fclean all

.PHONY: all clean fclean re