#include <stdio.h>
#include <stdlib.h>

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

int	main(int argc, char **argv)
{
	parse_error(argc, argv);
	return (0);
}