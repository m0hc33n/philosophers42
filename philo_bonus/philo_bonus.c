#include "philo_bonus.h"

static void	eat(t_philo *philo)
{
	sem_wait(philo->lc->sem_pool);
	stdlog(philo, TAKEFORK);
	sem_wait(philo->lc->sem_pool);
	stdlog(philo, TAKEFORK);
	sem_wait(philo->lc->sem_meal_check);
	stdlog(philo, ISEATING);
	philo->last_meal_tv = get_current_time();
	sem_post(philo->lc->sem_meal_check);
	usleep(philo->lc->tte * 1000);
	sem_post(philo->lc->sem_pool);
	sem_post(philo->lc->sem_pool);
}

static void	sleep_think(t_philo *philo)
{
	stdlog(philo, ISSLEEPING);
	usleep(philo->lc->tts * 1000);
	stdlog(philo, ISTHINKING);
	if (philo->local_shifts.is_set)
		philo->local_shifts.shifts_nbr--;
}

void	*philosophers(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	philo->last_meal_tv = get_current_time();
	pthread_create(&philo->pthread_tracer, NULL,
		philo_monitor, philo);
	if (philo->id % 2 == 0)
		usleep(500);
	while (!philo->philo_die)
	{
		if (philo->local_shifts.is_set && !philo->local_shifts.shifts_nbr)
			exit(PHILOF);
		eat(philo);
		sleep_think(philo);
	}
	pthread_join(philo->pthread_tracer, NULL);
	if (philo->philo_die)
		exit(PHILOD);
	exit(PHILOF);
	return (NULL);
}
