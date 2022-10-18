/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 11:34:18 by rgarcia           #+#    #+#             */
/*   Updated: 2022/10/18 16:02:31 by rgarcia          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "philo.h"

void	print_fork(t_philo *args_philo, int fork)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&args_philo->updating);
	if (args_philo->is_dead == 0)
		printf("%ld %d has taken a fork\n", \
			(1000 * tv.tv_sec + tv.tv_usec / 1000) - \
			args_philo->zero_time, args_philo->philo_seat);
	else
	{
		if (fork != 2)
			pthread_mutex_unlock(args_philo->own_fork);
		if (fork != 1)
			pthread_mutex_unlock(args_philo->right_fork);
	}
	pthread_mutex_unlock(&args_philo->updating);
}

int	take_fork(t_philo *args_philo)
{
	if (args_philo->nb_philo == 1)
	{
		pthread_mutex_lock(args_philo->own_fork);
		print_fork(args_philo, 1);
		return (1);
	}
	else if (((args_philo->philo_seat % 2) == 0) && \
		(args_philo->philo_seat + 1 != args_philo->nb_philo - 1))
	{
		pthread_mutex_lock(args_philo->right_fork);
		print_fork(args_philo, 2);
		pthread_mutex_lock(args_philo->own_fork);
		print_fork(args_philo, 3);
	}
	else
	{
		pthread_mutex_lock(args_philo->own_fork);
		print_fork(args_philo, 1);
		pthread_mutex_lock(args_philo->right_fork);
		print_fork(args_philo, 3);
	}
	return (2);
}

void	eating(t_philo *args_philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&args_philo->updating);
	args_philo->last_update = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	printf("%ld %d is eating\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - \
		args_philo->zero_time, args_philo->philo_seat);
	if (args_philo->nb_eat > 0)
		args_philo->nb_eat -= 1;
	pthread_mutex_unlock(&args_philo->updating);
	ft_usleep(args_philo->time_eat);
	pthread_mutex_unlock(args_philo->own_fork);
	pthread_mutex_unlock(args_philo->right_fork);
}

void	sleeping(t_philo *args_philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&args_philo->updating);
	printf("%ld %d is sleeping\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - \
		args_philo->zero_time, args_philo->philo_seat);
	pthread_mutex_unlock(&args_philo->updating);
	ft_usleep(args_philo->time_sleep);
}

void	thinking(t_philo *args_philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&args_philo->updating);
	printf("%ld %d is thinking\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - \
		args_philo->zero_time, args_philo->philo_seat);
	pthread_mutex_unlock(&args_philo->updating);
}
