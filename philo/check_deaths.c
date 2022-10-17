#include <stdio.h>
#include <stdlib.h>
#include "philo.h"

void	starved(t_philo *dead_philo)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&dead_philo->printing);
	printf("%ld %d died\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - dead_philo->zero_time, dead_philo->philo_seat);
	pthread_mutex_unlock(&dead_philo->printing);
}

int	check_time_death(t_philo *philo)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&philo->updating);
	if ((1000 * tv.tv_sec + tv.tv_usec / 1000) - philo->last_update > philo->time_die)
	{
		pthread_mutex_unlock(&philo->updating);
		return (1);
	}
	pthread_mutex_unlock(&philo->updating);
	return (0);
}

int	count_eat(t_arguments *args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (args->nb_eat <= 0)
		return (0);
	while (i < args->nb_philo)
	{
		pthread_mutex_lock(&args->philos[i].counting);
			if (args->philos[i].nb_eat == 0)
				j = j + 1;
		pthread_mutex_unlock(&args->philos[i].counting);
		i++;
	}
	if (j == args->nb_philo)
		return (1);
	return (0);
}

int	check_deaths(t_arguments *args)
{
	int	i;
	int	j;

	i = 0;
	while (i < args->nb_philo)
	{
		if (args->nb_eat > 0)
		{
			if (count_eat(args) == 1)
				return (1);
		}
		if (check_time_death(&args->philos[i]) == 1)
		{
			j = 0;
			while (j < args->nb_philo)
			{
				pthread_mutex_lock(&args->philos[j].death_check);
				args->philos[j].is_dead = 1;
				pthread_mutex_unlock(&args->philos[j].death_check);
				j++;
			}
			starved(&args->philos[i]);
			return (1);
		}
		i++;
	}
	return (0);
}