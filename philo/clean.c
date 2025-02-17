#include "philo.h"

// FREE PHILO LINKED LIST (MOK)
void	philo_free(t_philo **philo)
{
	t_philo	*head;
	t_philo	*tmp;

	if (philo && *philo)
	{
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

// philo_free && CLEAN UP INIT MUTEXES
void	philo_exit(t_philo **philo, t_lifecycle **lc)
{
	if (lc && *lc)
	{
		if ((*lc)->stdlog_mutex.mutex_initialized)
			pthread_mutex_destroy(&(*lc)->stdlog_mutex.mutex);
		if ((*lc)->meal_check.mutex_initialized)
			pthread_mutex_destroy(&(*lc)->meal_check.mutex);
		if ((*lc)->death_check.mutex_initialized)
			pthread_mutex_destroy(&(*lc)->death_check.mutex);
		memset(*lc, 0, sizeof(t_lifecycle));
		free(*lc);
		*lc = NULL;
	}
	if (philo && *philo)
	{
		while (true)
		{
			if ((*philo)->fork_mutex.mutex_initialized)
				pthread_mutex_destroy(&(*philo)->fork_mutex.mutex);
			*philo = (*philo)->flink;
			if ((*philo)->id == 1)
				break ;
		}
		philo_free(philo);
	}
}
