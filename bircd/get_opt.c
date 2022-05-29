
#include <stdio.h>
#include <stdlib.h>
#include "bircd.h"

void	get_opt(t_env *e, int ac, char **av)
{
  if (ac != 3)
    {
      fprintf(stderr, USAGE, av[0]);
      exit(1);
    }
  e->port[0] = atoi(av[1]);
  e->port[1] = atoi(av[2]);
}

