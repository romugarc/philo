/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgarcia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 16:14:55 by rgarcia           #+#    #+#             */
/*   Updated: 2022/10/29 14:47:56 by rgarcia          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>

typedef struct s_philo
{
	int				nb_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nb_eat;
	int				philo_seat;
	long			last_update;
	int				is_dead;
	long			zero_time;
	pthread_mutex_t	updating;
	pthread_mutex_t	*own_fork;
	pthread_mutex_t	*right_fork;
}	t_philo;

typedef struct s_arguments
{
	int				nb_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nb_eat;
	long			big_bang_time;
	pthread_mutex_t	updating;
	pthread_mutex_t	*mutexes;
	pthread_t		*threads;
	t_philo			*philos;
}	t_arguments;

int		ft_atoi(const char *str);
int		parse_error(int argc, char **argv);
int		parse_args(int nb_arg, char **argv, t_arguments *args);
int		create_threads(t_arguments *args);
int		create_philos(t_arguments *args);
void	create_philos_updates(t_arguments *args, t_philo *philo, int i);
int		create_mutex(t_arguments *args);
int		create_everything(t_arguments *args);
void	*start_routine(void *arg);
int		vitals_check(t_philo *philo);
int		check_deaths(t_arguments *args);
int		check_time_deaths(t_philo *philo, int args_nb_eat, int *j);
void	everyone_dies(t_arguments *args);
int		take_fork(t_philo *args_philo);
void	print_fork(t_philo *args_philo, int fork);
void	eating(t_philo *args_philo);
void	sleeping(t_philo *args_philo);
void	thinking(t_philo *args_philo);
void	starved(t_philo *dead_philo);
void	ft_usleep(int time_ms);
void	end_routine(t_arguments *args, int err_state);
void	free_all(t_arguments *args);
void	destroy_mutex(t_arguments *args, int err_state);
int		error_handler(t_arguments *args, int err_state);

#endif
