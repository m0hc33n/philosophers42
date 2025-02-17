CC 		= cc 
CFLAGS	= -Wall -Wextra -Werror -lpthread

SRC	= main.c philo.c init.c monitor.c tools.c clean.c mutex_op.c
HDR	= philo.h
OBJ	= $(SRC:.c=.o)

all : philosophers

philosophers : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o philosophers

$(OBJ) : $(HDR)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f philosophers

re : fclean all

.PHONY : clean all fclean re
