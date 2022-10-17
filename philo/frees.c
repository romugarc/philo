#include "philo.h"

void	free_all(t_arguments *args)
{
	free(args->philos);
	free(args->mutexes);
	free(args->threads);
}

void	destroy_mutex(t_arguments *args)
{
	int	i;

	i = 0;
	while (i < args->nb_philo)
	{
		pthread_mutex_lock(&args->mutexes[i]);
		pthread_mutex_unlock(&args->mutexes[i]);
		pthread_mutex_destroy(&args->mutexes[i]);
		pthread_mutex_lock(&args->philos[i].death_check);
		pthread_mutex_unlock(&args->philos[i].death_check);
		pthread_mutex_destroy(&args->philos[i].death_check);
		pthread_mutex_lock(&args->philos[i].counting);
		pthread_mutex_unlock(&args->philos[i].counting);
		pthread_mutex_destroy(&args->philos[i].counting);
		pthread_mutex_lock(&args->philos[i].updating);
		pthread_mutex_unlock(&args->philos[i].updating);
		pthread_mutex_destroy(&args->philos[i].updating);
		i++;
	}
}

void	end_routine(t_arguments *args)
{
	int	i;

	pthread_mutex_lock(&args->printing);
	i = 0;
	while (i < args->nb_philo)
	{
		pthread_join(args->threads[i], NULL);
		i++;
	}
	destroy_mutex(args);
	pthread_mutex_unlock(&args->printing);
	pthread_mutex_destroy(&args->printing);
	free_all(args);
}
