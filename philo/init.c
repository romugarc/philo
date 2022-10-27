/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 16:14:41 by rgarcia           #+#    #+#             */
/*   Updated: 2022/10/27 18:28:22 by rgarcia          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_mutex(t_arguments *args)
{
	pthread_mutex_t	*mutex;
	int				i;

	mutex = malloc(sizeof(pthread_mutex_t) * args->nb_philo);
	if (!mutex)
	{
		args->mutexes = mutex;
		return (1);
	}
	i = -1;
	while (i++ < args->nb_philo - 1)
	{
		if (pthread_mutex_init(&mutex[i], NULL) != 0)
		{
			args->mutexes = mutex;
			return (1);
		}
	}
	if (pthread_mutex_init(&args->updating, NULL) != 0)
	{
		args->mutexes = mutex;
		return (2);
	}
	args->mutexes = mutex;
	return (2);
	return (0);
}

void	create_philos_updates(t_arguments *args, t_philo *philos, int i)
{
	struct timeval	tv;

	philos[i].nb_philo = args->nb_philo;
	philos[i].time_die = args->time_die;
	philos[i].time_eat = args->time_eat;
	philos[i].time_sleep = args->time_sleep;
	philos[i].nb_eat = args->nb_eat;
	philos[i].philo_seat = i + 1;
	philos[i].own_fork = &args->mutexes[i];
	philos[i].zero_time = args->big_bang_time;
	gettimeofday(&tv, NULL);
	philos[i].last_update = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	philos[i].is_dead = 0;
	philos[i].updating = args->updating;
}

int	create_philos(t_arguments *args)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * args->nb_philo);
	if (!philos)
	{
		free(philos);
		return (1);
	}
	i = 0;
	while (i < args->nb_philo)
	{
		create_philos_updates(args, philos, i);
		if (pthread_mutex_init(&philos->updating, NULL) != 0)
			return (1);
		if (i > 0)
			philos[i].right_fork = &args->mutexes[i - 1];
		else
			philos[i].right_fork = &args->mutexes[args->nb_philo - 1];
		i++;
	}
	args->philos = philos;
	return (0);
}

int	create_threads(t_arguments *args)
{
	pthread_t	*thread;
	int			i;

	thread = malloc(sizeof(pthread_t) * args->nb_philo);
	if (!thread)
	{
		free(thread);
		return (1);
	}
	i = 0;
	while (i < args->nb_philo)
	{
		pthread_create(&thread[i], NULL, start_routine, \
			(void *)&args->philos[i]);
		i++;
	}
	args->threads = thread;
	return (0);
}
