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

void	take_fork(t_philo *args_philo)
{
	pthread_mutex_lock(args_philo->own_fork);
	pthread_mutex_lock(args_philo->right_fork);
	printf("%d has taken a fork\n", args_philo->philo_seat);
}

void	eating(t_philo *args_philo)
{
	printf("%d is eating\n", args_philo->philo_seat);
	args_philo->nb_eat += 1;
	usleep(args_philo->time_eat * 1000);
	pthread_mutex_unlock(args_philo->own_fork);
	pthread_mutex_unlock(args_philo->right_fork);
}

void	sleeping(t_philo *args_philo)
{
	printf("%d is sleeping\n", args_philo->philo_seat);
	usleep(args_philo->time_sleep * 1000);
}

void	thinking(t_philo *args_philo)
{
	printf("%d is thinking\n", args_philo->philo_seat);
}

void	starved(t_philo *args_philo)
{
	printf("%d died\n", args_philo->philo_seat);
}

void	*start_routine(void *arg)
{
	t_philo *a;
	int		b;

	a = (t_philo *)a;
	b = a->time_die;
	//take_fork(a);
	/*eating((t_philo *)arg);
	sleeping((t_philo *)arg);
	thinking((t_philo *)arg);
	*/printf("s");
	printf("%d\n", b);
}

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
	args->mutexes = mutex;
}

void	create_philos(t_arguments *args)
{
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
		if (i != 0)
			philos[i].right_fork = &args->mutexes[i - 1];
		else
			philos[i].right_fork = &args->mutexes[args->nb_philo];
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
		printf("%d\n", args->philos[i].philo_seat);
		pthread_create(&thread[i], NULL, start_routine, (void *)&args->philos[i]);
		i++;
	}
	args->threads = thread;
}

int	main(int argc, char **argv)
{
	int			nb_arg;
	int			i;
	t_arguments args;
	t_philo *sauce;

	if (parse_error(argc, argv) == 1)
		return (0);
	nb_arg = argc - 1;
	if (parse_args(nb_arg, argv, &args) == 1)
		return (0);
	create_mutex(&args);
	create_philos(&args);
	i = 0;
	while (i < args.nb_philo)
	{
		printf("%d\n", args.philos[i].philo_seat);
		i++;
	}
	create_threads(&args);
	i = 0;
	while (i < args.nb_philo)
	{
		pthread_join(args.threads[i], NULL);
		i++;
	}
	i = 0;
	while (i < args.nb_philo)
	{
		pthread_mutex_unlock(&args.mutexes[i]);
		pthread_mutex_destroy(&args.mutexes[i]);
		i++;
	}
	free(args.philos);
	free(args.mutexes);
	free(args.threads);
	return (0);
}
