/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 16:24:56 by rgarcia           #+#    #+#             */
/*   Updated: 2022/10/18 15:17:19 by rgarcia          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	vitals_check(t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philo->updating);
	if (philo->is_dead == 1 || philo->nb_eat == 0)
	{
		i = 1;
	}
	pthread_mutex_unlock(&philo->updating);
	return (i);
}

void	*start_routine(void *arg)
{
	t_philo	*a;
	int		locks;

	a = (t_philo *)arg;
	if ((a->philo_seat) % 2 == 1)
		usleep(9000);
	locks = 0;
	while (1)
	{
		if (vitals_check(a) != 1 && locks != 1)
			locks = take_fork(a);
		if (vitals_check(a) == 1)
			return (NULL);
		if (locks == 2)
			eating(a);
		if (vitals_check(a) == 1)
			return (NULL);
		if (locks == 2)
			sleeping(a);
		if (vitals_check(a) == 1)
			return (NULL);
		if (locks == 2)
			thinking(a);
	}
	return (NULL);
}
