#include "philo_bonus.h"

/*
 * NOTE :
 * 	if die -> lock stdout using sem_wait(sem_stdlog)
 * 	to prevent other philo to write to stdout
*/
void	*philo_monitor(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo)
	{
		while (true)
		{
			if (philo->local_shifts.is_set && !philo->local_shifts.shifts_nbr)
				exit(PHILOF);
			sem_wait(philo->lc->sem_meal_check);
			if (philo->last_meal_tv
				&& get_current_time() - philo->last_meal_tv > philo->lc->ttd)
			{
				stdlog(philo, DIED);
				sem_wait(philo->lc->sem_stdlog);
				philo->philo_die = true;
				sem_post(philo->lc->sem_meal_check);
				exit(PHILOD);
			}
			sem_post(philo->lc->sem_meal_check);
		}
	}
	return (NULL);
}
