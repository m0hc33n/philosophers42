#include "philo_bonus.h"

void	destroy_sem(t_lifecycle *lc)
{
	sem_close(lc->sem_stdlog);
	sem_close(lc->sem_pool);
	sem_close(lc->sem_death);
	sem_close(lc->sem_stop);
	sem_unlink(SEMLOG);
	sem_unlink(SEMPOOL);
	sem_unlink(SEMDEATH);
	sem_unlink(SEMSTOP);
}

static void	philo_kill(t_philo *philo)
{
	uint64_t	i;

	i = 0;
	if (philo)
	{
		while (i < philo->lc->philo_nbr)
		{
			kill(philo[i].pid, SIGKILL);
			i++;
		}
	}
}

void	philo_exit(t_philo **philo, t_lifecycle *lc)
{
	philo_kill(*philo);
	destroy_sem(lc);
	free(*philo);
}
