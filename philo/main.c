#include "philo.h"

static void	err(t_status errnum)
{
	printf("\n[!!] ERROR : %#08x\n\n", errnum);
}

static void	usage(void)
{
	printf("Usage:\n\t"
		"./philosophers <philo> <ttd> <tte> <tts> [shifts]\n"
		"\nphilo  : number of philosophers"
		"\nttd    : time to die"
		"\ntte    : time to eat"
		"\ntts    : time to sleep"
		"\n\n[optional]"
		"\nshifts : number of times each philosopher must eat"
		"\n");
}

/*
 * CREATE PHILO TRACER (PHILO MONITOR)
 * CREATE PHILO THREADS
*/
static t_status	go(t_philo **philo)
{
	if (pthread_create(&(*philo)->lc->pthread_tracer, NULL,
			philo_monitor, *philo) != 0)
		return (THRERROR);
	(*philo)->lc->start_tv = get_current_time();
	while (true)
	{
		if (pthread_create(&(*philo)->pthread, NULL, philosophers, *philo) != 0)
			return (THRERROR);
		philo_set_uint64(
			&(*philo)->last_meal_tv,
			(*philo)->lc->start_tv,
			&(*philo)->lc->meal_check.mutex);
		(*philo) = (*philo)->flink;
		if ((*philo)->id == 1)
			break ;
	}
	pthread_join((*philo)->lc->pthread_tracer, NULL);
	while (true)
	{
		pthread_join((*philo)->pthread, NULL);
		(*philo) = (*philo)->flink;
		if ((*philo)->id == 1)
			break ;
	}
	return (SUCCESS);
}

/*
	MAIN()
*/
int	main(int ac, char **av)
{
	t_philo		*philo;
	t_lifecycle	*lc;
	t_status	status;

	philo = NULL;
	lc = NULL;
	status = philo_init(&philo, &lc, ac, av);
	if (status)
		return (err(status), usage(), philo_exit(&philo, &lc), FAILURE);
	status = go(&philo);
	if (status)
		return (err(status), usage(), philo_exit(&philo, &lc), FAILURE);
	philo_exit(&philo, &lc);
	return (SUCCESS);
}
