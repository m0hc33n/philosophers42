CC 		= cc 
CFLAGS	= -Wall -Wextra -Werror -lpthread

SRC	= main_bonus.c philo_bonus.c init_bonus.c monitor_bonus.c tools_bonus.c
HDR	= philo_bonus.h
OBJ	= $(SRC:.c=.o)

all : philosophers_bonus

philosophers_bonus : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o philosophers_bonus

$(OBJ) : $(HDR)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f philosophers_bonus

re : fclean all

.PHONY : clean all fclean re
