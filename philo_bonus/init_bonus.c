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
static int32_t	philo_lifecycle(t_lifecycle *lc, int ac, char **av)
{
	if (ac == 5 || ac == 6)
	{
		memset(lc, 0, sizeof(t_lifecycle));
		if (!av_extractor(lc, av))
			return (ARGERROR);
		lc->sem_stop = sem_open(SEMSTOP, O_CREAT, S_IRUSR | S_IWUSR, 1);
		lc->sem_stdlog = sem_open(SEMLOG, O_CREAT, S_IRUSR | S_IWUSR, 1);
		lc->sem_death = sem_open(SEMDEATH, O_CREAT, S_IRUSR | S_IWUSR, 1);
		lc->sem_pool = sem_open(SEMPOOL, O_CREAT, S_IRUSR | S_IWUSR,
				lc->philo_nbr);
		if (lc->sem_pool == SEM_FAILED || lc->sem_stdlog == SEM_FAILED
			|| lc->sem_death == SEM_FAILED || lc->sem_stop == SEM_FAILED)
			return (SEMERROR);
		if (ac == 6)
		{
			if (!fetch_value(av[5], &lc->global_shifts.shifts_nbr))
				return (ARGERROR);
			lc->global_shifts.is_set = true;
			lc->global_shifts_nbr = lc->philo_nbr
				* lc->global_shifts.shifts_nbr;
		}
		return (SUCCESS);
	}
	return (ARGERROR);
}

static t_status	init_philo(t_philo **philo, t_lifecycle *lc)
{
	uint64_t	i;

	*philo = (t_philo *)malloc(lc->philo_nbr * sizeof(t_philo));
	if (!*philo)
		return (MALERROR);
	memset(*philo, 0, sizeof(t_philo) * lc->philo_nbr);
	i = 0;
	while (i < lc->philo_nbr)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].lc = lc;
		if (lc->global_shifts.is_set)
			(*philo)->local_shifts.is_set = true;
		i++;
	}
	return (SUCCESS);
}

/*
 *
*/
t_status	philo_init(t_philo **philo, t_lifecycle *lc, int ac, char **av)
{
	int32_t	status;

	destroy_sem(lc);
	status = philo_lifecycle(lc, ac, av);
	if (status)
		return (status);
	status = init_philo(philo, lc);
	if (status)
		return (status);
	return (SUCCESS);
}
