#include "philo_bonus.h"

static void	*monitor(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	while (true)
	{
		sem_wait(philo->lc->sem_death);
		if (get_current_time() - philo->last_meal_tv > philo->lc->ttd)
		{
			stdlog(philo, DIED);
			sem_post(philo->lc->sem_stop);
			break ;
		}
		if (philo->local_shifts.is_set)
		{
			if (philo->local_shifts.shifts_nbr >= philo->lc->global_shifts_nbr)
			{
				sem_post(philo->lc->sem_stop);
				break ;
			}
		}
		sem_post(philo->lc->sem_death);
		usleep(philo->lc->ttd * 100);
	}
	return (NULL);
}

static void	philo_think_eat(t_philo *philo)
{
	stdlog(philo, ISTHINKING);
	sem_wait(philo->lc->sem_pool);
	stdlog(philo, TAKEFORK);
	sem_wait(philo->lc->sem_pool);
	stdlog(philo, TAKEFORK);
	stdlog(philo, ISEATING);
	sem_wait(philo->lc->sem_death);
	if (philo->local_shifts.is_set)
		philo->local_shifts.shifts_nbr++;
	philo->last_meal_tv = get_current_time();
	sem_post(philo->lc->sem_death);
	usleep(philo->lc->tte * 1000);
	sem_post(philo->lc->sem_pool);
	sem_post(philo->lc->sem_pool);
}

static void	philo_sleep(t_philo *philo)
{
	stdlog(philo, ISSLEEPING);
	usleep(philo->lc->tts * 1000);
}

void	philosophers(t_philo *philo)
{
	pthread_t	monitorid;

	philo->last_meal_tv = get_current_time();
	if (pthread_create(&monitorid, NULL, monitor, (void *)philo) != 0)
	{
		stdlog(philo, DIED);
		sem_post(philo->lc->sem_stop);
		return ;
	}
	pthread_detach(monitorid);
	while (true)
	{
		philo_think_eat(philo);
		philo_sleep(philo);
	}
}
