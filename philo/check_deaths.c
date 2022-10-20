/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_deaths.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 16:13:59 by rgarcia           #+#    #+#             */
/*   Updated: 2022/10/20 09:28:35 by rgarcia          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "philo.h"

void	starved(t_philo *dead_philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&dead_philo->updating);
	printf("%ld %d died\n", (1000 * tv.tv_sec + tv.tv_usec / 1000) - \
		dead_philo->zero_time, dead_philo->philo_seat);
	pthread_mutex_unlock(&dead_philo->updating);
}

int	check_time_deaths(t_philo *philo, int args_nb_eat, int *j)
{
	struct timeval	tv;

	pthread_mutex_lock(&philo->updating);
	if (args_nb_eat > 0)
	{
		if (philo->nb_eat <= 0)
			*j = *j + 1;
	}
	gettimeofday(&tv, NULL);
	if ((1000 * tv.tv_sec + tv.tv_usec / 1000) - \
		philo->last_update > philo->time_die)
	{
		pthread_mutex_unlock(&philo->updating);
		return (1);
	}
	pthread_mutex_unlock(&philo->updating);
	return (0);
}

void	everyone_dies(t_arguments *args)
{
	int	j;

	j = 0;
	while (j < args->nb_philo)
	{
		pthread_mutex_lock(&args->philos[j].updating);
		args->philos[j].is_dead = 1;
		pthread_mutex_unlock(&args->philos[j].updating);
		j++;
	}
}

int	check_deaths(t_arguments *args)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (i++ < args->nb_philo - 1)
	{
		if (check_time_deaths(&args->philos[i], args->nb_eat, &j) == 1 || \
			j == args->nb_philo)
		{
			everyone_dies(args);
			if (j != args->nb_philo)
				starved(&args->philos[i]);
			return (1);
		}
		if (i == args->nb_philo)
			j = 0;
	}
	return (0);
}
