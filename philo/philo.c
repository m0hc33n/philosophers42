#include "philo.h"

static bool	philo_shifts_finish(t_philo *philo)
{
	uint64_t	r;

	if (philo->lc->global_shifts.is_set)
	{
		r = philo_dec_uint64(
			&philo->local_shifts.shifts_nbr,
			&philo->lc->meal_check.mutex,
			1);
		if (!r)
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

static bool	philo_take_forks(t_philo *philo)
{
	if (philo->flink == philo)
	{
	 	usleep(philo->lc->ttd + 100);
	 	return (false);
	}
	pthread_mutex_lock(&philo->fork_mutex.mutex);
	stdlog(philo, TAKEFORK);
	pthread_mutex_lock(&philo->flink->fork_mutex.mutex);
	stdlog(philo, TAKEFORK);
	return (true);
}

static bool	philo_eat(t_philo *philo)
{
	stdlog(philo, ISTHINKING);
	if (!philo_take_forks(philo))
		return (false);
	stdlog(philo, ISEATING);
	pthread_mutex_lock(&philo->lc->meal_check.mutex);
	philo->last_meal_tv = get_current_time();
	pthread_mutex_unlock(&philo->lc->meal_check.mutex);
	usleep(philo->lc->tte * 1000);
	pthread_mutex_unlock(&philo->fork_mutex.mutex);
	pthread_mutex_unlock(&philo->flink->fork_mutex.mutex);
	return (true);
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
	if (philo->id % 2)
		usleep(1000);
	while (true)
	{
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
		if (philo->local_shifts.is_set && philo_shifts_finish(philo))
			break ;
		if (is_philo_die(philo->lc))
			break ;
	}
	return (NULL);
}
