#include "philo.h"

uint64_t	philo_get_uint64(void *var, pthread_mutex_t *pmutex)
{
	uint64_t	val;

	pthread_mutex_lock(pmutex);
	val = *(uint64_t *)var;
	pthread_mutex_unlock(pmutex);
	return (val);
}

void	philo_dec_uint64(void *var, pthread_mutex_t *pmutex, uint64_t dec)
{
	pthread_mutex_lock(pmutex);
	*(uint64_t *)var -= dec;
	pthread_mutex_unlock(pmutex);
}

void	philo_set_uint64(void *var, uint64_t val, pthread_mutex_t *pmutex)
{
	pthread_mutex_lock(pmutex);
	*(uint64_t *)var = val;
	pthread_mutex_unlock(pmutex);
}
