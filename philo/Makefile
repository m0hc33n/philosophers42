CC 		= cc 
CFLAGS	= -Wall -Wextra -Werror
SRC	= main.c philo.c init.c monitor.c tools.c clean.c mutex_op.c
HDR	= philo.h
OBJ	= $(SRC:.c=.o)

all : philosophers	

philosophers : $(OBJ)
	$(CC) $(CFLAGS) -lpthread $(OBJ) -o philo

$(OBJ) : $(HDR)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f philo

re : fclean all

.PHONY : clean all fclean re
