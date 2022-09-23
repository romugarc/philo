#include <stdio.h>
#include <stdlib.h>

s_args	args
{
	int	nb_philo;
	int	time_eat;
	int	time_die;
}	t_args;

int	parse_error(int narg, char **argv)
{
	int	i;
	int	j;

	if (narg != 5)
		return (1);
	i = 1;
	j = 0;
	while (argv[i] != NULL)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (argv[i][j] <= '0' || argv[i][j] >= '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

t_args	init_args(nb_arg, argv)
{
	return ();
}

int	main(int argc, char **argv)
{
	int	nb_arg;
	t_args	args;

	nb_arg = argc - 1;
	parse_error(argc, argv);
	init_args(nb_arg, argv);
	return (0);
}
