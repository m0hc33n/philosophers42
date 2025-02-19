#include "philo_bonus.h"

// LOG TO STDOUT
void	stdlog(t_philo *philo, const char *msg)
{
	size_t	ct;

	if (philo)
	{
		sem_wait(philo->lc->sem_stdlog);
		ct = get_current_time() - philo->lc->start_tv;
		printf("%012lu %lu %s\n", ct, philo->id, msg);
		if (*msg != 'd')
			sem_post(philo->lc->sem_stdlog);
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
	if (*arg || !res)
		return (false);
	*value = res;
	return (true);
}

void	err(t_status status)
{
	printf("\n[!!] ERROR : %#08x\n\n", status);
}
