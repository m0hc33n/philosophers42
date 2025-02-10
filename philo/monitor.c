#include "philo.h"

bool	is_philo_die(t_lifecycle *lc)
{
	bool	fdie;

	fdie = false;
	pthread_mutex_lock(&lc->death_check.mutex);
	if (lc->global_shifts.is_set)
	{
		if (!lc->global_shifts_nbr)
			fdie = true;
	}
	if (lc->philo_die)
		fdie = true;
	pthread_mutex_unlock(&lc->death_check.mutex);
	if (fdie)
		return (true);
	return (false);
}

static bool	death_monitor(t_philo *philo)
{
	size_t	tv;

	pthread_mutex_lock(&philo->lc->meal_check.mutex);
	tv = get_current_time();
	if (philo->last_meal_tv
		&& tv - philo->last_meal_tv > philo->lc->ttd)
	{
		stdlog(philo, DIED);
		pthread_mutex_lock(&philo->lc->death_check.mutex);
		philo->lc->philo_die = true;
		pthread_mutex_unlock(&philo->lc->death_check.mutex);
		pthread_mutex_unlock(&philo->lc->meal_check.mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->lc->meal_check.mutex);
	return (false);
}

void	*philo_monitor(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo)
	{
		while (true)
		{
			if (is_philo_die(philo->lc))
				return (NULL);
			if (philo->lc->global_shifts.is_set)
			{
				if (!philo->local_shifts.shifts_nbr)
				{
					philo = philo->flink;
					continue ;
				}
			}
			if (death_monitor(philo))
				return (NULL);
			philo = philo->flink;
		}
	}
	return (NULL);
}
