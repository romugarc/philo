#include <stdio.h>
#include <stdlib.h>
#include "philo.h"

int	parse_error(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc - 1 < 4 || argc - 1 > 5)
		return (1);
	i = 1;
	j = 0;
	while (argv[i] != NULL)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int parse_args(int nb_arg, char** argv, t_arguments *args)
{
	args->nb_philo = ft_atoi(argv[1]);
	args->time_die = ft_atoi(argv[2]);
	args->time_eat = ft_atoi(argv[3]);
	args->time_sleep = ft_atoi(argv[4]);
	if (args->nb_philo <= 0)
		return (1);
	if (nb_arg == 5)
	{
		args->nb_eat = ft_atoi(argv[5]);
		if (args->nb_eat <= 0)
			return (1);
	}
	else
		args->nb_eat = -1;
	return (0);
}

void	ft_usleep(int time_ms)
{
	struct timeval tv;
	long	start_time;
	long	delayed_time;

	gettimeofday(&tv, NULL);
	start_time = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	delayed_time = start_time;
	while (delayed_time - start_time < time_ms)
	{
		usleep(time_ms / 10);
		gettimeofday(&tv, NULL);
		delayed_time = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	}
}

int	main(int argc, char **argv)
{
	int			nb_arg;
	int			i;
	t_arguments args;
	struct timeval tv;

	if (parse_error(argc, argv) == 1)
		return (0);
	nb_arg = argc - 1;
	if (parse_args(nb_arg, argv, &args) == 1)
		return (0);
	create_mutex(&args);
	gettimeofday(&tv, NULL);
	args.big_bang_time = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	create_philos(&args);
	create_threads(&args);
	while (1)
 	{
		if (check_deaths(&args) == 1)
		{
			pthread_mutex_lock(&args.printing);
			i = 0;
			while (i < args.nb_philo)
			{
				pthread_join(args.threads[i], NULL);
				i++;
			}
			i = 0;
			while (i < args.nb_philo)
			{
				pthread_mutex_lock(&args.mutexes[i]);
				pthread_mutex_unlock(&args.mutexes[i]);
				pthread_mutex_destroy(&args.mutexes[i]);
				pthread_mutex_lock(&args.philos[i].death_check);
				pthread_mutex_unlock(&args.philos[i].death_check);
				pthread_mutex_destroy(&args.philos[i].death_check);
				pthread_mutex_lock(&args.philos[i].counting);
				pthread_mutex_unlock(&args.philos[i].counting);
				pthread_mutex_destroy(&args.philos[i].counting);
				pthread_mutex_lock(&args.philos[i].updating);
				pthread_mutex_unlock(&args.philos[i].updating);
				pthread_mutex_destroy(&args.philos[i].updating);
				i++;
			}
			pthread_mutex_unlock(&args.printing);
			pthread_mutex_destroy(&args.printing);
			free(args.philos);
			free(args.mutexes);
			free(args.threads);
			return (0);
		}
	}
	return (0);
}