/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 16:14:08 by rgarcia           #+#    #+#             */
/*   Updated: 2022/10/28 15:49:57 by rgarcia          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_all(t_arguments *args)
{
	free(args->philos);
	free(args->mutexes);
	free(args->threads);
}

void	destroy_mutex(t_arguments *args, int err_state)
{
	int	i;

	i = 0;
	while (i < args->nb_philo)
	{
		if (err_state == 0 || err_state == 2)
			pthread_mutex_lock(&args->philos[i].updating);
		pthread_mutex_unlock(&args->mutexes[i]);
		pthread_mutex_destroy(&args->mutexes[i]);
		if (err_state == 0 || err_state == 2)
			pthread_mutex_unlock(&args->philos[i].updating);
		if (err_state == 0 || err_state == 2)
			pthread_mutex_destroy(&args->philos[i].updating);
		i++;
	}
}

int	error_handler(t_arguments *args, int err_state)
{
	if (err_state == 2)
	{
		destroy_mutex(args, 1);
		free(args->mutexes);
	}
	else if (err_state == 3)
	{
		end_routine(args, 1);
		free(args->mutexes);
	}
	else if (err_state == 4 || err_state == 5)
	{
		end_routine(args, 2);
		free(args->mutexes);
		free(args->philos);
	}
	else if (err_state == 6)
	{
		everyone_dies(args);
		end_routine(args, 0);
	}
	return (1);
}

void	end_routine(t_arguments *args, int err_state)
{
	int	i;

	pthread_mutex_lock(&args->updating);
	if (err_state == 0)
	{
		i = 0;
		while (i < args->nb_philo)
		{
			pthread_join(args->threads[i], NULL);
			i++;
		}
	}
	destroy_mutex(args, err_state);
	pthread_mutex_unlock(&args->updating);
	pthread_mutex_destroy(&args->updating);
	if (err_state == 0)
		free_all(args);
}
