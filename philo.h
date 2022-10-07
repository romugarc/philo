#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_philo
{
    int nb_philo;
    int time_die;
    int time_eat;
    int time_sleep;
    int nb_eat;
    int philo_seat;
    pthread_mutex_t *own_fork;
    pthread_mutex_t *right_fork;
}   t_philo;

typedef struct s_arguments
{
    int nb_philo;
    int time_die;
    int time_eat;
    int time_sleep;
    int nb_eat;
    pthread_mutex_t *mutexes;
    pthread_t       *threads;
    t_philo         *philos;
}   t_arguments;

int	ft_atoi(const char *str);

#endif