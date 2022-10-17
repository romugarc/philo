#include "philo.h"

void	create_mutex(t_arguments *args)
{
	pthread_mutex_t *mutex;
	int				i;

	mutex = malloc(sizeof(pthread_mutex_t) * args->nb_philo);
	if (!mutex)
		return ;
	i = 0;
	while (i < args->nb_philo)
	{
		pthread_mutex_init(&mutex[i], NULL);
		i++;
	}
	pthread_mutex_init(&args->printing, NULL);
	args->mutexes = mutex;
}

void	create_death_count_update_mutexes(t_philo *philo)
{
	pthread_mutex_init(&philo->death_check, NULL);
	pthread_mutex_init(&philo->counting, NULL);
	pthread_mutex_init(&philo->updating, NULL);
}

void	create_philos(t_arguments *args)
{
	struct timeval tv;
	t_philo *philos;
	int		i;

	philos = malloc(sizeof(t_philo) * args->nb_philo);
	if (!philos)
		return ;
	i = 0;
	while (i < args->nb_philo)
	{
		philos[i].nb_philo = args->nb_philo;
		philos[i].time_die = args->time_die;
		philos[i].time_eat = args->time_eat;
		philos[i].time_sleep = args->time_sleep;
		philos[i].nb_eat = args->nb_eat;
		philos[i].philo_seat = i;
		philos[i].own_fork = &args->mutexes[i];
		if (i > 0)
			philos[i].right_fork = &args->mutexes[i - 1];
		else
			philos[i].right_fork = &args->mutexes[args->nb_philo - 1];
		philos[i].zero_time = args->big_bang_time;
		gettimeofday(&tv, NULL);
		philos[i].last_update = 1000 * tv.tv_sec + tv.tv_usec / 1000;
		philos[i].is_dead = 0;
		philos[i].printing = args->printing;
		create_death_count_update_mutexes(&philos[i]);
		i++;
	}
	args->philos = philos;
}

void	create_threads(t_arguments *args)
{

	pthread_t 	*thread;
	int			i;

	thread = malloc(sizeof(pthread_t) * args->nb_philo);
	if (!thread)
		return ;
	i = 0;
	while (i < args->nb_philo)
	{
		pthread_create(&thread[i], NULL, start_routine, (void *)&args->philos[i]);
		i++;
	}
	args->threads = thread;
}