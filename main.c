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

void	right_fork(t_philo *args_philo, int fork)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
		pthread_mutex_lock(&args_philo->printing);
	pthread_mutex_lock(&args_philo->death_check);
	if (args_philo->is_dead == 0)
		printf("%ld %d has taken right fork\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - args_philo->zero_time, args_philo->philo_seat);
	else
	{
		if (fork == 2)
			pthread_mutex_unlock(args_philo->own_fork);
		//if (args_philo->nb_philo > 1)
		pthread_mutex_unlock(args_philo->right_fork);
	}
	pthread_mutex_unlock(&args_philo->death_check);
	pthread_mutex_unlock(&args_philo->printing);
}

void	own_fork(t_philo *args_philo, int fork)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
		pthread_mutex_lock(&args_philo->printing);
	pthread_mutex_lock(&args_philo->death_check);
	if (args_philo->is_dead == 0)
		printf("%ld %d has taken own fork\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - args_philo->zero_time, args_philo->philo_seat);
	else
	{
		pthread_mutex_unlock(args_philo->own_fork);
		//if (args_philo->nb_philo > 1)
		if (fork == 2)
			pthread_mutex_unlock(args_philo->right_fork);
	}
	pthread_mutex_unlock(&args_philo->death_check);
	pthread_mutex_unlock(&args_philo->printing);
}

void	take_fork(t_philo *args_philo)
{
	if (((args_philo->philo_seat % 2) == 0) && (args_philo->philo_seat + 1  != args_philo->nb_philo - 1))
	{
		pthread_mutex_lock(args_philo->right_fork);
		right_fork(args_philo, 1);
		pthread_mutex_lock(args_philo->own_fork);
		own_fork(args_philo, 2);
	}
	else
	{
		pthread_mutex_lock(args_philo->own_fork);
		own_fork(args_philo, 1);
	//if (args_philo->nb_philo > 1)
		pthread_mutex_lock(args_philo->right_fork);
		right_fork(args_philo, 2);
	}
}

void	eating(t_philo *args_philo)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&args_philo->updating);
	args_philo->last_update = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	pthread_mutex_unlock(&args_philo->updating);
	pthread_mutex_lock(&args_philo->printing);
	printf("%ld %d is eating\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - args_philo->zero_time, args_philo->philo_seat);
	pthread_mutex_unlock(&args_philo->printing);
	pthread_mutex_lock(&args_philo->counting);
	if (args_philo->nb_eat > 0)
		args_philo->nb_eat -= 1;
	pthread_mutex_unlock(&args_philo->counting);
	ft_usleep(args_philo->time_eat);
	//usleep(args_philo->time_eat * 1000);
	pthread_mutex_unlock(args_philo->own_fork);
	//if (args_philo->nb_philo != 1)
	pthread_mutex_unlock(args_philo->right_fork);
}

void	sleeping(t_philo *args_philo)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&args_philo->printing);
	printf("%ld %d is sleeping\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - args_philo->zero_time, args_philo->philo_seat);
	pthread_mutex_unlock(&args_philo->printing);
	ft_usleep(args_philo->time_sleep);
}

void	thinking(t_philo *args_philo)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&args_philo->printing);
	printf("%ld %d is thinking\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - args_philo->zero_time, args_philo->philo_seat);
	pthread_mutex_unlock(&args_philo->printing);
}

void	starved(t_philo *dead_philo)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&dead_philo->printing);
	printf("%ld %d died\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - dead_philo->zero_time, dead_philo->philo_seat);
	pthread_mutex_unlock(&dead_philo->printing);
}

int	check_own_death(t_philo *philo)
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
	//	printf("%d\t", 1);
		if (vitals_check(a) == 1)
			return (NULL);
		eating(a);
	//	printf("%d\t", 2);
		if (vitals_check(a) == 1)
			return (NULL);
		sleeping(a);
	//	printf("%d\t", 3);
		if (vitals_check(a) == 1)
			return (NULL);
		thinking(a);
	//	printf("%d\t", 4);
	}
	return (NULL);
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
	//printf("%d\n", 1);
	while (i < args->nb_philo)
	{
		if (args->nb_eat > 0)
		{
			if (count_eat(args) == 1)
				return (1);
		}
		//printf("%d\n", 2);
		if (check_own_death(&args->philos[i]) == 1)
		{
			//printf("%d\n", 3);
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

void	create_threads(t_arguments *args)
{
//	pthread_t	monitor;
//	pthread_t	eats;
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
//	pthread_create(&monitor, NULL, monitor_deaths, (void *)&args);
//	pthread_join(monitor, NULL);
//	if (args->nb_eat > 0)
//	{
//		pthread_create(&eats, NULL, monitor_eat, (void *)&args);
//		pthread_join(eats, NULL);
//	}
	args->threads = thread;
}

int	main(int argc, char **argv)
{
	int			nb_arg;
	int			i;
	//int			b;
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
	//b = 0;
	while (1)
 	{
		if (check_deaths(&args) == 1)
		{
			pthread_mutex_lock(&args.printing);
		//	pthread_detach(args.monitor);
		//	if (args.nb_eat > 0)
		//		pthread_detach(args.eats);
			//printf("death\t");
			i = 0;
			while (i < args.nb_philo)
			{
				//printf("join\t");
				pthread_join(args.threads[i], NULL);
			//	pthread_detach(args.threads[i]);
				//printf("join2\t");
				i++;
			}
			i = 0;
			while (i < args.nb_philo)
			{
				//printf("mutex\t");
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
			//printf("frees\t");
			return (0);
		}
	}
	return (0);
}