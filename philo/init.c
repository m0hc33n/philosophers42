#include "philo.h"

static t_status	av_mutex_init(t_lifecycle *lc, char **av)
{
	if (!fetch_value(av[1], &lc->philo_nbr)
		|| !fetch_value(av[2], &lc->ttd)
		|| !fetch_value(av[3], &lc->tte)
		|| !fetch_value(av[4], &lc->tts)
	)
		return (ARGERROR);
	if (pthread_mutex_init(&lc->stdlog_mutex.mutex, NULL) != 0)
		return (MUTERROR);
	lc->stdlog_mutex.mutex_initialized = true;
	if (pthread_mutex_init(&lc->meal_check.mutex, NULL) != 0)
		return (MUTERROR);
	lc->meal_check.mutex_initialized = true;
	if (pthread_mutex_init(&lc->death_check.mutex, NULL) != 0)
		return (MUTERROR);
	lc->death_check.mutex_initialized = true;
	return (SUCCESS);
}

// SET s_lifecycle STRUCT
// ITS A SHARED STRUCT ACROSS ALL PHILOS
static t_status	philo_lifecycle(t_lifecycle **lc, int ac, char **av)
{
	t_status	status;

	if (ac == 5 || ac == 6)
	{
		*lc = (t_lifecycle *)malloc(sizeof(t_lifecycle));
		if (!lc)
			return (MALERROR);
		memset(*lc, 0, sizeof(t_lifecycle));
		status = av_mutex_init(*lc, av);
		if (status)
			return (status);
		if (ac == 6)
		{
			if (!fetch_value(av[5], &(*lc)->global_shifts.shifts_nbr))
				return (ARGERROR);
			(*lc)->global_shifts.is_set = true;
			(*lc)->global_shifts_nbr = (*lc)->philo_nbr
				* (*lc)->global_shifts.shifts_nbr;
		}
		return (SUCCESS);
	}
	return (ARGERROR);
}

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
		if (pthread_mutex_init(&p->fork_mutex.mutex, NULL) != 0)
			return (MUTERROR);
		p->fork_mutex.mutex_initialized = true;
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
	t_status	status;

	status = philo_lifecycle(lc, ac, av);
	if (status)
		return (status);
	status = init_table(philo, *lc);
	if (status)
		return (status);
	return (SUCCESS);
}
