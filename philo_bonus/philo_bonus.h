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
# define SEMLOG "/philosem_log" // sem name for stdout logs
# define SEMPOOL "/philosem_pool" // sem name for forks pool
# define SEMMEAL "/philosem_meal"

// PID && PPID
# define CHILDPID 0x0

// STATUS CODE
/*
	2   : PHILOD (exit code of a philo die)
	3   : PHILOF (exit code of a philo phinish shifts)
	100 : PIDERROR (fork faild to create a child process)
	101 : SEMERROR (SEM FAILED)
	102 : ARGERROR (incorrect args, see usage)
*/
typedef enum e_status
{
	SUCCESS		= 0x000000,
	FAILURE		= 0x000001,
	ARGERROR	= 0x000002,
	MALERROR	= 0x000003,
	PIDERROR	= 0x000004,
	SEMERROR 	= 0x000005,
	PHILOD		= 0x000006,
	PHILOF		= 0x000007,
}	t_status;

// number_of_times_each_philosopher_must_eat
typedef struct s_shifts
{
	bool		is_set;
	uint64_t	shifts_nbr;
}	t_shifts;

/*
 * id				<> philosopher id
 * ppid				<> parent pid
 * pid				<> philo pid
 * local_shifts		<> remaining shifts to eat
 * last_meal_tv		<> self explainatory
 * philo_die		<> when a philo die, its sets to true
 * lc				<> to use semaphores;
 * flink			<> next philo
 * blink			<> prev philo
*/
typedef struct s_philo
{
	int32_t				id;
	int32_t				ppid;
	int32_t				pid;
	t_shifts			local_shifts;
	size_t				last_meal_tv;
	bool				philo_die;
	struct s_lifecycle	*lc;
	struct s_philo		*flink;
	struct s_philo		*blink;
}	t_philo;

/*
 * philo_nbr		<> number_of_philosophers
 * ttd				<> time_to_die
 * tte				<> timeto_eat
 * tts				<> time_to_sleep
 * global_shifts	<> number_of_times_each_philosopher_must_eat
 * sem_pool			<> sem used as forks
 * sem_meal_check	<> sem used to
*/
typedef struct s_lifecycle
{
	uint64_t		philo_nbr;
	uint64_t		ttd;
	uint64_t		tte;
	uint64_t		tts;
	t_shifts		global_shifts;
	sem_t			*sem_pool;
	sem_t			*sem_stdlog;
	sem_t			*sem_meal_check;
	size_t			start_tv;
}	t_lifecycle;

// PHILO INIT
t_status	philo_init(t_philo **philo, t_lifecycle **lc,
				int ac, char **av);
bool		fetch_value(char *arg, uint64_t *value);
void		philo_exit(t_philo **philo, t_lifecycle **lc);

// PHILO
void	*philosophers(void *philo);
//void	*philo_monitor(void *philo);
bool	is_philo_die(t_philo *philo);
bool	philo_shifts_finish(t_philo *philo);

// TOOLS
void	stdlog(t_philo *philo, const char *msg);
size_t	get_current_time(void);
void	err(t_status errnum);
void	destroy_sem(t_lifecycle *lc);

#endif