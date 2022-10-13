#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
    int nb_philo;
    int time_die;
    int time_eat;
    int time_sleep;
    int nb_eat;
    int philo_seat;
    long    last_update;
    int is_dead;
    long    zero_time;
    pthread_mutex_t counting;
    pthread_mutex_t death_check;
    pthread_mutex_t updating;
    pthread_mutex_t printing;
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
    long big_bang_time;
    pthread_mutex_t printing;
    pthread_mutex_t *mutexes;
    pthread_t       *threads;
//    pthread_t       monitor;
 //   pthread_t       eats;
    t_philo         *philos;
}   t_arguments;

int	ft_atoi(const char *str);

#endif