#include "philo.h"

// LOG TO STDOUT
void	stdlog(t_philo *philo, const char *msg)
{
	size_t	ct;

	if (philo && !is_philo_die(philo->lc))
	{
		pthread_mutex_lock(&philo->lc->stdlog_mutex.mutex);
		ct = get_current_time() - philo->lc->start_tv;
		printf("%010lu %d %s\n", ct, philo->id, msg);
		pthread_mutex_unlock(&philo->lc->stdlog_mutex.mutex);
	}
}

// 	RETURN CURRENT TIME ON MILLISECONDS
size_t	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

// TO UINT64_T (PROGRAM ARGS)
bool	fetch_value(char *arg, uint64_t *value)
{
	uint64_t	res;

	res = 0;
	while (*arg == SPACE)
		arg++;
	if (*arg == PLUS)
		arg++;
	while (*arg >= 48 && *arg <= 57)
	{
		res = (*arg - ZERO) + (res * 10);
		arg++;
	}
	while (*arg == SPACE)
		arg++;
	if (*arg)
		return (false);
	*value = res;
	return (true);
}

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
