#include "philo_bonus.h"

void	destroy_sem(t_lifecycle *lc)
{
	sem_close(lc->sem_stdlog);
	sem_close(lc->sem_pool);
	sem_close(lc->sem_meal_check);
	sem_unlink(SEMLOG);
	sem_unlink(SEMPOOL);
	sem_unlink(SEMMEAL);
}

static void	philo_free(t_philo **philo)
{
	t_philo	*head;
	t_philo	*tmp;

	if (philo && *philo)
	{
		memset((*philo)->lc, 0, sizeof(t_lifecycle));
		free((*philo)->lc);
		(*philo)->lc = NULL;
		head = *philo;
		*philo = (*philo)->flink;
		while ((*philo)->id > 1)
		{
			tmp = (*philo)->flink;
			memset(*philo, 0, sizeof(t_philo));
			free(*philo);
			*philo = tmp;
		}
		tmp = NULL;
		memset(head, 0, sizeof(t_philo));
		free(head);
		head = NULL;
		*philo = NULL;
	}
}

static void	philo_kill(t_philo *philo)
{
	if (philo)
	{
		while (true)
		{
			kill(philo->pid, SIGTERM);
			philo = philo->flink;
			if (philo->id == 1)
				break;
		}
	}
}

void	philo_exit(t_philo **philo, t_lifecycle **lc)
{
	int32_t		status;
	uint64_t	i;
	int32_t		exit_status;

	if (philo && *philo)
	{
		i = 0;
		while (i < (*philo)->lc->philo_nbr)
		{
			waitpid(-1, &status, 0);
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			if (exit_status == PHILOD)
			{
				philo_kill(*philo);
				break ;
			}
				i++;
		}
		destroy_sem(*lc);
		philo_free(philo);
	}
}
