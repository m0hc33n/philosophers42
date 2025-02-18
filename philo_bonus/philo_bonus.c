#include "philo_bonus.h"

static bool	is_philo_die(t_philo *philo)
{
	sem_wait(philo->lc->sem_death);
	if (philo->last_meal_tv
		&& get_current_time() - philo->last_meal_tv > philo->lc->ttd)
	{
		stdlog(philo, DIED);
		return (true);
	}
	sem_post(philo->lc->sem_death);
	return (false);
}

static void	philo_eat(t_philo *philo)
{
	sem_wait(philo->lc->sem_pool);
	stdlog(philo, TAKEFORK);
	sem_wait(philo->lc->sem_pool);
	stdlog(philo, TAKEFORK);
	stdlog(philo, ISEATING);
	if (philo->local_shifts.is_set)
		philo->local_shifts.shifts_nbr--;
	sem_wait(philo->lc->sem_death);
	philo->last_meal_tv = get_current_time();
	sem_post(philo->lc->sem_death);
	usleep(philo->lc->tte * 1000);
	sem_post(philo->lc->sem_pool);
	sem_post(philo->lc->sem_pool);
}

static void	philo_think(t_philo *philo)
{
	stdlog(philo, ISTHINKING);
}

static void	philo_sleep(t_philo *philo)
{
	stdlog(philo, ISSLEEPING);
	usleep(philo->lc->tts * 1000);
}

void	philosophers(t_philo *philo, uint64_t index)
{
	t_philo	*curr_philo;

	curr_philo = (t_philo *)(philo + index);
	curr_philo->last_meal_tv = get_current_time();
	while (curr_philo->local_shifts.shifts_nbr)
	{
		philo_think(curr_philo);
		philo_eat(curr_philo);
		philo_sleep(curr_philo);
		if (is_philo_die(curr_philo))
			child_exit(philo, PHILOD);
	}
	child_exit(philo, PHILOF);
}
