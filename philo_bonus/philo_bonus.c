#include "philo_bonus.h"

static void	philo_eat(t_philo *philo)
{
	sem_wait(philo->lc->sem_pool);
	stdlog(philo, TAKEFORK);
	sem_wait(philo->lc->sem_pool);
	stdlog(philo, TAKEFORK);
	stdlog(philo, ISEATING);
	philo->last_meal_tv = get_current_time();
	usleep(philo->lc->tte * 1000);
	sem_post(philo->lc->sem_pool);
	sem_post(philo->lc->sem_pool);
}

static bool	philo_think(t_philo *philo)
{
	stdlog(philo, ISTHINKING);
	if (philo->lc->philo_nbr == 1)
		usleep(philo->lc->ttd * 1000 + 100);
	if (is_philo_die(philo))
		return (false);
	return (true);
}

static bool	philo_sleep(t_philo *philo)
{
	stdlog(philo, ISSLEEPING);
	if (is_philo_die(philo))
		return (false);
	usleep(philo->lc->tts * 1000);
	return (true);
}

// static void	init_philo(t_philo *philo, t_philo *p)
// {
// 	philo->lc = p->lc;
// 	philo->id = p->id;
// 	philo->local_shifts = p->local_shifts;
// 	philo->philo_die = false;
// 	philo->last_meal_tv = get_current_time();
// }

void	*philosophers(void *p)
{
	t_philo		*philo;

	//init_philo(&philo, (t_philo *)p);
	// philo = (t_philo *)p;
	// philo->last_meal_tv = get_current_time();
	// if (philo->id % 2 == 0)
	// 	usleep(15000);
	// while (true)
	// {
	// 	if (!philo_think(philo))
	// 		break ;
	// 	philo_eat(philo);
	// 	if (!philo_sleep(philo))
	// 		break ;
	// 	if (is_philo_die(philo))
	// 		break ;
	// 	if (philo_shifts_finish(philo))
	// 		break;
	// }
	// if (philo->philo_die)
	// 	exit(PHILOD);
	// exit(PHILOF);
	return (NULL);
}
