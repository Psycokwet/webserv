#include <stdlib.h>
#include <netdb.h> // getprotobyname
#include <netinet/in.h>
#include "bircd.h"

void			srv_create(t_env *e, int port[])
{
  int			s;
  int			s_2;
  struct sockaddr_in	sin;
  struct sockaddr_in	sin_2;
  struct protoent	*pe;
  
  pe = (struct protoent*)Xv(NULL, getprotobyname("tcp"), "getprotobyname");

  s = X(-1, socket(PF_INET, SOCK_STREAM, pe->p_proto), "socket");
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port[0]);
  X(-1, bind(s, (struct sockaddr*)&sin, sizeof(sin)), "bind");
  X(-1, listen(s, 42), "listen"); // maximum 42 users in the pending queue
  e->fds[s].type = FD_SERV;
  e->fds[s].chatroom = ROOM_A;
  e->fds[s].fct_read = srv_accept;

  s_2 = X(-1, socket(PF_INET, SOCK_STREAM, pe->p_proto), "socket");
  sin_2.sin_family = AF_INET;
  sin_2.sin_addr.s_addr = INADDR_ANY;
  sin_2.sin_port = htons(port[1]);
  X(-1, bind(s_2, (struct sockaddr*)&sin_2, sizeof(sin_2)), "bind");
  X(-1, listen(s_2, 42), "listen"); // maximum 42 users in the pending queue
  e->fds[s_2].type = FD_SERV;
  e->fds[s_2].chatroom = ROOM_B;
  e->fds[s_2].fct_read = srv_accept;
}
