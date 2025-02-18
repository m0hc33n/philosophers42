#include "philo_bonus.h"

static void	usage(void)
{
	printf("Usage:\n\t"
		"./philosophers_bonus <philo> <ttd> <tte> <tts> [shifts]\n"
		"\nphilo  : number of philosophers"
		"\nttd    : time to die"
		"\ntte    : time to eat"
		"\ntts    : time to sleep"
		"\n\n[optional]"
		"\nshifts : number of times each philosopher must eat"
		"\n");
}

/*
 * # create each philo on a separate child process
*/

static t_status	go(t_philo *philo)
{
	int32_t		pid;
	uint64_t	i;

	i = 0;
	philo->lc->start_tv = get_current_time();
	while (i < philo->lc->philo_nbr)
	{
		pid = fork();
		if (pid < 0)
			return (PIDERROR);
		else if (pid == CHILDPID)
			philosophers(philo, i);
		else
			philo[i].pid = pid;
		i++;
		usleep(100);
	}
	return (SUCCESS);
}

/*
	MAIN()
*/
int	main(int ac, char **av)
{
	t_philo		*philo;
	t_lifecycle	lc;
	t_status	status;

	philo = NULL;
	status = philo_init(&philo, &lc, ac, av);
	if (status)
		return (err(status), usage(), philo_exit(&philo, &lc), FAILURE);
	status = go(philo);
	if (status)
		return (err(status), philo_exit(&philo, &lc), FAILURE);
	philo_exit(&philo, &lc);
	return (SUCCESS);
}
