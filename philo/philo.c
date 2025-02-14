#include "philo.h"

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_mutex.mutex);
	stdlog(philo, TAKEFORK);
	pthread_mutex_lock(&philo->flink->fork_mutex.mutex);
	stdlog(philo, TAKEFORK);
	pthread_mutex_lock(&philo->lc->meal_check.mutex);
	stdlog(philo, ISEATING);
	philo->last_meal_tv = get_current_time();
	pthread_mutex_unlock(&philo->lc->meal_check.mutex);
	usleep(philo->lc->tte * 1000);
	pthread_mutex_unlock(&philo->fork_mutex.mutex);
	pthread_mutex_unlock(&philo->flink->fork_mutex.mutex);
}

static bool	sleep_think(t_philo *philo)
{
	stdlog(philo, ISSLEEPING);
	usleep(philo->lc->tts * 1000);
	stdlog(philo, ISTHINKING);
	if (philo->lc->global_shifts.is_set)
	{
		philo->local_shifts.shifts_nbr--;
		if (!philo->local_shifts.shifts_nbr)
		{
			pthread_mutex_lock(&philo->lc->death_check.mutex);
			philo->lc->global_shifts_nbr -= philo->lc->global_shifts.shifts_nbr;
			pthread_mutex_unlock(&philo->lc->death_check.mutex);
			return (false);
		}
	}
	return (true);
}

void	*philosophers(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo->id % 2 == 0)
		usleep(500);
	while (true)
	{
		eat(philo);
		if (is_philo_die(philo->lc))
			break ;
		if (!sleep_think(philo))
			break ;
	}
	return (NULL);
}
