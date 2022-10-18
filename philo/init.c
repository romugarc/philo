/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 16:14:41 by rgarcia           #+#    #+#             */
/*   Updated: 2022/10/18 11:29:35 by rgarcia          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_mutex(t_arguments *args)
{
	pthread_mutex_t	*mutex;
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
	pthread_mutex_init(&args->updating, NULL);
	args->mutexes = mutex;
}

void	create_philos_updates(t_arguments *args, t_philo *philos, int i)
{
	struct timeval	tv;

	philos[i].zero_time = args->big_bang_time;
	gettimeofday(&tv, NULL);
	philos[i].last_update = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	philos[i].is_dead = 0;
	philos[i].updating = args->updating;
	pthread_mutex_init(&philos->updating, NULL);
}

void	create_philos(t_arguments *args)
{
	t_philo	*philos;
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
		create_philos_updates(args, philos, i);
		i++;
	}
	args->philos = philos;
}

void	create_threads(t_arguments *args)
{
	pthread_t	*thread;
	int			i;

	thread = malloc(sizeof(pthread_t) * args->nb_philo);
	if (!thread)
		return ;
	i = 0;
	while (i < args->nb_philo)
	{
		pthread_create(&thread[i], NULL, start_routine, \
			(void *)&args->philos[i]);
		i++;
	}
	args->threads = thread;
}
