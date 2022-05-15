
#include "bircd.h"

int	main(int ac, char **av)
{
  t_env	e;

  init_env(&e); // init variabel e
  get_opt(&e, ac, av); // get port from av[1]
  srv_create(&e, e.port); // socket, bind, listen and set fd is FD_SERV
  main_loop(&e);
  return (0);
}
