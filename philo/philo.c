#include "philo.h"

static bool	philo_shifts_finish(t_philo *philo)
{
	if (philo->lc->global_shifts.is_set)
	{
		philo_dec_uint64(
			&philo->local_shifts.shifts_nbr,
			&philo->lc->meal_check.mutex,
			1);
		if (!philo_get_uint64(&philo->local_shifts.shifts_nbr,
				&philo->lc->meal_check.mutex))
		{
			philo_dec_uint64(
				&philo->lc->global_shifts_nbr,
				&philo->lc->death_check.mutex,
				philo->lc->global_shifts.shifts_nbr);
			return (true);
		}
	}
	return (false);
}

static bool	philo_think(t_philo *philo)
{
	stdlog(philo, ISTHINKING);
	if (&philo->flink->fork_mutex.mutex == &philo->fork_mutex.mutex)
		usleep(philo->lc->ttd * 1000 + 10);
	return (true);
}

static void	philo_eat(t_philo *philo)
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

static bool	philo_sleep(t_philo *philo)
{
	stdlog(philo, ISSLEEPING);
	if (is_philo_die(philo->lc))
		return (false);
	usleep(philo->lc->tts * 1000);
	return (true);
}

void	*philosophers(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo->id % 2 == 0)
		usleep(10000);
	while (true)
	{
		if (!philo_think(philo))
			break ;
		if (is_philo_die(philo->lc))
			return (false);
		philo_eat(philo);
		if (!philo_sleep(philo))
			break ;
		if (philo->local_shifts.is_set && philo_shifts_finish(philo))
			break ;
	}
	return (NULL);
}
