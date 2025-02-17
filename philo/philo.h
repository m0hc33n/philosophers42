#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <stdint.h>

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

// STATUS CODE
// ARGERROR		: see usage;
// MALERROR		: malloc() failed;
// MUTERROR		: pthread_mutex_init failed;
// THRERROR		: pthread_create failed;
typedef enum e_status
{
	SUCCESS		= 0x000000,
	FAILURE		= 0x000001,
	ARGERROR	= 0x000002,
	MALERROR	= 0x000003,
	MUTERROR	= 0x000004,
	THRERROR	= 0X000005
}	t_status;

// number_of_times_each_philosopher_must_eat
typedef struct s_shifts
{
	bool		is_set;
	uint64_t	shifts_nbr;
}	t_shifts;

// mutex init
typedef struct s_mutex
{
	bool			mutex_initialized;
	pthread_mutex_t	mutex;
}	t_mutex;

/*
 * philo_nbr		<> number_of_philosophers
 * ttd				<> time_to_die
 * tte				<> timeto_eat
 * tts				<> time_to_sleep
 * global_shifts	<> number_of_times_each_philosopher_must_eat
 * global_shifts_nbr<> global_shifts.shifts_nbr;
 * philo_die		<> when a philo die, its sets to true
 * pthread_tracer	<> a thread to trace philo life `philo_monitor`
 * stdlog_mutex		<> stdout sync `stdlog`
*/
typedef struct s_lifecycle
{
	uint64_t		philo_nbr;
	uint64_t		ttd;
	uint64_t		tte;
	uint64_t		tts;
	t_shifts		global_shifts;
	uint64_t		global_shifts_nbr;
	bool			philo_die;
	pthread_t		pthread_tracer;
	t_mutex			stdlog_mutex;
	t_mutex			meal_check;
	t_mutex			death_check;
	uint64_t		start_tv;
}	t_lifecycle;

/*
 * id			<> philosopher id
 * philo_shifts <> remaining shifts to eat
 * lifecycle	<> see above
 * flink		<> right side philosopher
 * blink		<> left  side philosopher
*/
typedef struct s_philo
{
	int32_t			id;
	t_shifts		local_shifts;
	pthread_t		pthread;
	t_mutex			fork_mutex;
	uint64_t		last_meal_tv;
	t_lifecycle		*lc;
	struct s_philo	*flink;
	struct s_philo	*blink;
}	t_philo;

// PHILO INIT
t_status	philo_init(t_philo **philo, t_lifecycle **lc,
				int ac, char **av);
bool		fetch_value(char *arg, uint64_t *value);
void		philo_free(t_philo **philo);
void		philo_exit(t_philo **philo, t_lifecycle **lc);

// PHILO
void		*philosophers(void *philo);
void		*philo_monitor(void *philo);
bool		is_philo_die(t_lifecycle *lc);

// TOOLS
void		stdlog(t_philo *philo, const char *msg);
size_t		get_current_time(void);
uint64_t	philo_get_uint64(void *var, pthread_mutex_t *pmutex);
void		philo_dec_uint64(void *var, pthread_mutex_t *pmutex, uint64_t dec);
void		philo_set_uint64(void *var, uint64_t val, pthread_mutex_t *pmutex);

#endif
