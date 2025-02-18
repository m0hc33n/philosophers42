#include "philo_bonus.h"

void	destroy_sem(t_lifecycle *lc)
{
	sem_close(lc->sem_stdlog);
	sem_close(lc->sem_pool);
	sem_close(lc->sem_death);
	sem_unlink(SEMLOG);
	sem_unlink(SEMPOOL);
	sem_unlink(SEMDEATH);
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
	int32_t		status;
	uint64_t	i;
	int32_t		exit_status;

	if (philo && *philo)
	{
		i = 0;
		while (i < lc->philo_nbr)
		{
			waitpid(0, &status, 0);
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			if (exit_status == PHILOD)
			{
				philo_kill(*philo);
				break ;
			}	
			i++;
		}
		destroy_sem(lc);
		free(*philo);
		*philo = NULL;
	}
}

void	child_exit(t_philo *philo, t_status status)
{
	sem_close(philo->lc->sem_stdlog);
	sem_close(philo->lc->sem_pool);
	sem_close(philo->lc->sem_death);
	free(philo);
	exit(status);
}
