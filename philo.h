#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <unistd.h>

typedef struct s_arguments
{
    int nb_philo;
    int time_die;
    int time_eat;
    int time_sleep;
    int nb_eat;
}   t_arguments;

int	ft_atoi(const char *str);

#endif