#include "philo.h"

int	vitals_check(t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philo->death_check);
	pthread_mutex_lock(&philo->counting);
	if (philo->is_dead == 1 || philo->nb_eat == 0)
		i = 1;
	pthread_mutex_unlock(&philo->death_check);
	pthread_mutex_unlock(&philo->counting);
	return (i);
}

void	*start_routine(void *arg)
{
	t_philo *a;

	a = (t_philo *)arg;
	if ((a->philo_seat) % 2 == 1 || a->philo_seat == a->nb_philo - 1)
		usleep(1500);
	while (1)
	{
		if (vitals_check(a) == 1)
			return (NULL);
		take_fork(a);
		if (vitals_check(a) == 1)
			return (NULL);
		eating(a);
		if (vitals_check(a) == 1)
			return (NULL);
		sleeping(a);
		if (vitals_check(a) == 1)
			return (NULL);
		thinking(a);
	}
	return (NULL);
}