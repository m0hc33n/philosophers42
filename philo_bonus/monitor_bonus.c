#include "philo_bonus.h"

bool	philo_shifts_finish(t_philo *philo)
{
	if (philo->local_shifts.is_set)
	{
		philo->local_shifts.shifts_nbr--;
		if (!philo->local_shifts.shifts_nbr)
			return (true);
	}
	return (false);
}

bool	is_philo_die(t_philo *philo)
{
	if (philo->last_meal_tv
		&& get_current_time() - philo->last_meal_tv > philo->lc->ttd)
	{
		stdlog(philo, DIED);
		philo->philo_die = true;
		return (true);
	}
	return (false);
}

