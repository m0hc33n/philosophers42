#include "philo_bonus.h"

static void	usage(void)
{
	printf("Usage:\n\t"
		"./philosophers <philo> <ttd> <tte> <tts> [shifts]\n"
		"\nphilo  : number of philosophers (must be gt 1 and le 200)"
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
static int32_t	go(t_lifecycle *lc)
{
	int32_t		pid;
	uint64_t	i;

	i = 1;
	lc->start_tv = get_current_time();
	while (i <= lc->philo_nbr)
	{
		pid = fork();
		if (pid < 0)
			return (PIDERROR);
		if (pid == CHILDPID)
			philosophers(&lc->philosophers[i]);
		lc->philosophers[i].pid = pid;
		usleep(1000);
		i++;
	}
	return (SUCCESS);
}

/*
	MAIN()
*/
int	main(int ac, char **av)
{
	t_lifecycle	lc;
	t_status	status;

	status = philo_init(&lc, ac, av);
	if (status)
		return (err(status), usage(), philo_exit(&lc), FAILURE);
	status = go(&lc);
	if (status)
		return (err(status), philo_exit(&lc), FAILURE);
	philo_exit(&lc);
	return (SUCCESS);
}
