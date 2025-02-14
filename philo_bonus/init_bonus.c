#include "philo_bonus.h"

static bool	av_extractor(t_lifecycle *lc, char **av)
{
	if (!fetch_value(av[1], &lc->philo_nbr)
		|| lc->philo_nbr < 2
		|| lc->philo_nbr > 200
		|| !fetch_value(av[2], &lc->ttd)
		|| !fetch_value(av[3], &lc->tte)
		|| !fetch_value(av[4], &lc->tts)
	)
		return (false);
	return (true);
}

// SET s_lifecycle STRUCT
// ITS A SHARED STRUCT ACROSS ALL PHILOS
static int32_t	philo_lifecycle(t_lifecycle *lc, int ac, char **av)
{
	if (ac == 5 || ac == 6)
	{
		memset(lc, 0, sizeof(t_lifecycle));
		if (!av_extractor(lc, av))
			return (ARGERROR);
		lc->sem_stdlog = sem_open(SEMLOG, O_CREAT, S_IRUSR | S_IWUSR, 1);
		lc->sem_pool = sem_open(SEMPOOL, O_CREAT, S_IRUSR | S_IWUSR,
				lc->philo_nbr);
		lc->sem_meal_check = sem_open(SEMMEAL, O_CREAT, S_IRUSR | S_IWUSR, 1);
		if (lc->sem_pool == SEM_FAILED || lc->sem_pool == SEM_FAILED
			|| lc->sem_meal_check == SEM_FAILED)
			return (SEMERROR);
		if (ac == 6)
		{
			if (!fetch_value(av[5], &lc->global_shifts.shifts_nbr))
				return (ARGERROR);
			lc->global_shifts.is_set = true;
		}
		return (SUCCESS);
	}
	return (ARGERROR);
}

static void	init_table(t_lifecycle *lc)
{
	uint64_t	i;
	int32_t		ppid;

	ppid = getpid();
	i = 1;
	while (i <= lc->philo_nbr)
	{
		lc->philosophers[i].id = i;
		lc->philosophers[i].ppid = ppid;
		lc->philosophers[i].local_shifts = lc->global_shifts;
		lc->philosophers[i].lc = lc;
		i++;
	}
}

/*
 * CHECK ARGS
 * INIT s_philo LINKED LIST (PHILOSOPHERS INFO)
 * NOTE: ITS A CIRCULAR BIDIRECTIONAL ONE
*/
int32_t	philo_init(t_lifecycle *lc, int ac, char **av)
{
	int32_t	lc_status;

	sem_unlink(SEMLOG);
	sem_unlink(SEMPOOL);
	sem_unlink(SEMMEAL);
	lc_status = philo_lifecycle(lc, ac, av);
	if (lc_status)
		return (lc_status);
	init_table(lc);
	return (SUCCESS);
}

void	philo_exit(t_lifecycle *lc)
{
	int32_t		status;
	uint64_t	i;
	int32_t		exit_status;

	i = 0;
	while (i < lc->philo_nbr)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		if (exit_status == PHILOD)
		{
			i = 1;
			while (i < lc->philo_nbr)
			{
				kill(lc->philosophers[i].pid, SIGTERM);
				i++;
			}
			break ;
		}
		i++;
	}
	destroy_sem(lc);
}
