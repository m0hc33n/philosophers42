#include "philo_bonus.h"

static bool	av_extractor(t_lifecycle *lc, char **av)
{
	if (!fetch_value(av[1], &lc->philo_nbr)
		|| !fetch_value(av[2], &lc->ttd)
		|| !fetch_value(av[3], &lc->tte)
		|| !fetch_value(av[4], &lc->tts)
	)
		return (false);
	return (true);
}

// SET s_lifecycle STRUCT
// ITS A SHARED STRUCT ACROSS ALL PHILOS
static int32_t	philo_lifecycle(t_lifecycle **lc, int ac, char **av)
{
	if (ac == 5 || ac == 6)
	{
		*lc = (t_lifecycle *)malloc(sizeof(t_lifecycle));
		if (!*lc)
			return (MALERROR);
		memset(*lc, 0, sizeof(t_lifecycle));
		if (!av_extractor(*lc, av))
			return (ARGERROR);
		(*lc)->sem_stdlog = sem_open(SEMLOG, O_CREAT, S_IRUSR | S_IWUSR, 1);
		(*lc)->sem_pool = sem_open(SEMPOOL, O_CREAT, S_IRUSR | S_IWUSR,
				(*lc)->philo_nbr);
		(*lc)->sem_meal_check = sem_open(SEMMEAL, O_CREAT, S_IRUSR | S_IWUSR, 1);
		if ((*lc)->sem_pool == SEM_FAILED || (*lc)->sem_pool == SEM_FAILED
			|| (*lc)->sem_meal_check == SEM_FAILED)
			return (SEMERROR);
		if (ac == 6)
		{
			if (!fetch_value(av[5], &(*lc)->global_shifts.shifts_nbr))
				return (ARGERROR);
			(*lc)->global_shifts.is_set = true;
		}
		return (SUCCESS);
	}
	return (ARGERROR);
}

// static void	init_table(t_lifecycle *lc)
// {
// 	uint64_t	i;
// 	int32_t		ppid;

// 	ppid = getpid();
// 	i = 1;
// 	while (i <= lc->philo_nbr)
// 	{
// 		lc->philosophers[i].id = i;
// 		lc->philosophers[i].ppid = ppid;
// 		lc->philosophers[i].local_shifts = lc->global_shifts;
// 		lc->philosophers[i].lc = lc;
// 		i++;
// 	}
// }

// init_table HELPER FUNCTION (SET s_philo MEMBERS)
static void	philo_linkit(t_philo **philo, t_philo *p)
{
	if (*philo)
	{
		p->flink = *philo;
		p->blink = (*philo)->blink;
		(*philo)->blink->flink = p;
		(*philo)->blink = p;
	}
	else
	{
		p->flink = p;
		p->blink = p;
		(*philo) = p;
	}
}


// SETUP A CIRCULAR BIDIRECTIONAL LINKED LIST
static t_status	init_table(t_philo **philo, t_lifecycle *lc)
{
	t_philo		*p;
	uint64_t	i;

	i = 1;
	while (i <= lc->philo_nbr)
	{
		p = (t_philo *)malloc(sizeof(t_philo));
		if (!p)
			return (MALERROR);
		memset(p, 0, sizeof(t_philo));
		p->lc = lc;
		p->local_shifts = lc->global_shifts;
		p->id = i;
		philo_linkit(philo, p);
		i++;
	}
	return (SUCCESS);
}


/*
 * CHECK ARGS
 * INIT s_philo LINKED LIST (PHILOSOPHERS INFO)
 * NOTE: ITS A CIRCULAR BIDIRECTIONAL ONE
*/
t_status	philo_init(t_philo **philo, t_lifecycle **lc, int ac, char **av)
{
	int32_t	status;

	sem_unlink(SEMLOG);
	sem_unlink(SEMPOOL);
	sem_unlink(SEMMEAL);
	status = philo_lifecycle(lc, ac, av);
	if (status)
		return (status);
	status = init_table(philo, *lc);
	if (status)
		return (status);
	return (SUCCESS);
}
