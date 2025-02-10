#include "philo.h"

static void	err(t_status errnum)
{
	printf("\n[!!] ERROR : %d\n\n", errnum);
}

static void	usage(void)
{
	printf("Usage:\n\t"
		"./philosophers <philo> <ttd> <tte> <tts> [shifts]\n"
		"\nphilo  : number of philosophers (must be gt 1)"
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
static void	go(t_philo **philo)
{
	size_t	ct;

	pthread_create(&(*philo)->lc->pthread_tracer, NULL,
		philo_monitor, *philo);
	ct = get_current_time();
	(*philo)->lc->start_tv = ct;
	while (true)
	{
		pthread_create(&(*philo)->pthread, NULL, philosophers, *philo);
		(*philo)->last_meal_tv = ct;
		(*philo) = (*philo)->flink;
		if ((*philo)->id == 1)
			break ;
		usleep(100);
	}
	pthread_join((*philo)->lc->pthread_tracer, NULL);
	while (true)
	{
		pthread_join((*philo)->pthread, NULL);
		(*philo) = (*philo)->flink;
		if ((*philo)->id == 1)
			break ;
	}
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
	go(&philo);
	philo_exit(&philo, &lc);
	return (SUCCESS);
}
