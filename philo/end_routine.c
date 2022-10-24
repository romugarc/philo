/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 16:14:08 by rgarcia           #+#    #+#             */
/*   Updated: 2022/10/24 17:47:33 by rgarcia          ###   ########lyon.fr   */
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
