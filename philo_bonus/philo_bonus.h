#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <fcntl.h>		 // O_CREAT
# include <semaphore.h>
# include <sys/types.h>
# include <wait.h>
# include <sys/stat.h>   // S_IRWXU
# include <stdbool.h>
# include <stdint.h>
# include <signal.h>

// ASCII
# define ZERO 48
# define SPACE 32
# define PLUS 43

// LOGS
# define TAKEFORK "has taken a fork"
# define ISEATING "is eating"
# define ISSLEEPING "is sleeping"
# define ISTHINKING "is thinking"
# define DIED "died"

// SEM NAMES
# define SEMLOG "/philosem_log"		//binary sem for stdout logs
# define SEMDEATH "/philosem_death" //binary sem for death check
# define SEMSTOP "/philosem_stop"	// binary sem for exit simulation
# define SEMPOOL "/philosem_pool"	//sem pool for forks pool

// PID && PPID
# define CHILDPID 0x0

// STATUS CODE
/*
	ARGERROR (incorrect args, see usage)
	MALERROR (malloc failed)
	PIDERROR (fork faild to create a child process)
	SEMERROR (SEM FAILED)
*/
typedef enum e_status
{
	SUCCESS		= 0x000000,
	FAILURE		= 0x000001,
	ARGERROR	= 0x000002,
	MALERROR	= 0x000003,
	PIDERROR	= 0x000004,
	SEMERROR	= 0x000005,
}	t_status;

// number_of_times_each_philosopher_must_eat
typedef struct s_shifts
{
	bool		is_set;
	uint64_t	shifts_nbr;
}	t_shifts;

/*
 * id				<> philosopher id
 * pid				<> philo pid
 * local_shifts		<> count shifts
 * last_meal_tv		<> self explainatory
 * lc				<> to use semaphores;
*/
typedef struct s_philo
{
	uint64_t			id;
	int32_t				pid;
	t_shifts			local_shifts;
	size_t				last_meal_tv;
	struct s_lifecycle	*lc;
}	t_philo;

/*
 * philo_nbr		<> number_of_philosophers
 * ttd				<> time_to_die
 * tte				<> timeto_eat
 * tts				<> time_to_sleep
 * global_shifts_nbr<> shifts_nbr * philo_nbr
 * global_shifts	<> number_of_times_each_philosopher_must_eat
*/
typedef struct s_lifecycle
{
	uint64_t		philo_nbr;
	uint64_t		ttd;
	uint64_t		tte;
	uint64_t		tts;
	uint64_t		global_shifts_nbr;
	t_shifts		global_shifts;
	sem_t			*sem_pool;
	sem_t			*sem_stdlog;
	sem_t			*sem_death;
	sem_t			*sem_stop;
	uint64_t		start_tv;
}	t_lifecycle;

// PHILO INIT
t_status	philo_init(t_philo **philo, t_lifecycle *lc,
				int ac, char **av);
bool		fetch_value(char *arg, uint64_t *value);
void		philo_exit(t_philo **philo, t_lifecycle *lc);

// PHILO
void		philosophers(t_philo *philo);

// TOOLS
void		stdlog(t_philo *philo, const char *msg);
size_t		get_current_time(void);
void		err(t_status errnum);
void		destroy_sem(t_lifecycle *lc);

#endif