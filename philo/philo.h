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
    t_philo         *philos;
}   t_arguments;

int	ft_atoi(const char *str);
void	create_threads(t_arguments *args);
void	create_philos(t_arguments *args);
void	create_death_count_update_mutexes(t_philo *philo);
void	create_mutex(t_arguments *args);
void	*start_routine(void *arg);
int vitals_check(t_philo *philo);
int	check_deaths(t_arguments *args);
int	count_eat(t_arguments *args);
int	check_time_death(t_philo *philo);
void	take_fork(t_philo *args_philo);
void	print_fork(t_philo *args_philo, int fork);
void	eating(t_philo *args_philo);
void	sleeping(t_philo *args_philo);
void	thinking(t_philo *args_philo);
void	starved(t_philo *dead_philo);
void	ft_usleep(int time_ms);

#endif