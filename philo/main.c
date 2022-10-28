/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 16:14:48 by rgarcia           #+#    #+#             */
/*   Updated: 2022/10/28 15:47:29 by rgarcia          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

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
	while (i < argc)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		if (j == 0)
			return (1);
		i++;
	}
	return (0);
}

int	parse_args(int nb_arg, char **argv, t_arguments *args)
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
	struct timeval	tv;
	long			start_time;
	long			delayed_time;

	gettimeofday(&tv, NULL);
	start_time = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	delayed_time = start_time;
	while (delayed_time - start_time < time_ms)
	{
		usleep(600);
		gettimeofday(&tv, NULL);
		delayed_time = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	}
}

int	create_everything(t_arguments *args)
{
	struct timeval	tv;
	int				err_state;

	err_state = create_mutex(args);
	if (err_state == 1)
		return (1);
	else if (err_state == 2)
		return (error_handler(args, err_state));
	gettimeofday(&tv, NULL);
	args->big_bang_time = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	err_state = create_philos(args);
	if (err_state == 3)
		return (error_handler(args, err_state));
	else if (err_state == 4)
		return (error_handler(args, err_state));
	err_state = create_threads(args);
	if (err_state == 5)
		return (error_handler(args, err_state));
	else if (err_state == 6)
		return (error_handler(args, err_state));
	return (0);
}

int	main(int argc, char **argv)
{
	int				nb_arg;
	t_arguments		args;

	if (parse_error(argc, argv) == 1)
		return (0);
	nb_arg = argc - 1;
	if (parse_args(nb_arg, argv, &args) == 1)
		return (0);
	if (create_everything(&args) == 1)
		return (0);
	while (1)
	{
		usleep(2000);
		if (check_deaths(&args) == 1)
		{
			end_routine(&args, 0);
			return (0);
		}
	}
	return (0);
}
