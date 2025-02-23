CC 		= cc
CFLAGS	= -Wall -Wextra -Werror

SRC	= main_bonus.c philo_bonus.c init_bonus.c tools_bonus.c clean_bonus.c
HDR	= philo_bonus.h
OBJ	= $(SRC:.c=.o)

all : philosophers_bonus

philosophers_bonus : $(OBJ)
	$(CC) $(CFLAGS) -lpthread $(OBJ) -o philo_bonus

$(OBJ) : $(HDR)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f philo_bonus

re : fclean all

.PHONY : clean all fclean re
