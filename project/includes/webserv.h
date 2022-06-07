/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-nguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#             */
/*   Updated: 2022/06/07 13:04:38 by thi-nguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H
# include <stdlib.h>
//malloc free
# include <unistd.h>
//write
# include <arpa/inet.h>
//htons htonl ntohs ntohl
# include <sys/select.h>
# include <sys/time.h>
# include <sys/types.h>
//select
# include <poll.h>
//poll
// # include <sys/epoll.h>
//epoll epoll_create epoll_ctl epoll_wait
// # include <sys/kqueue.h> not working for some reasons on my vm, should try at school
//kqueue kevent

# include <sys/socket.h>
//socket accept listen send recv bind connect setsockopt getsockname

# include <netinet/in.h>
//inet_addr 
#include <fcntl.h>
//fcntl

#include <sys/resource.h> // struct rlimit, getrlimit

# include <iostream>

#define MAX_CLIENTS 25
#define MAX_CLIENT_QUEUE 3
#define LOCALHOST  2130706433
#define BUF_SIZE 100000

#define FD_MAX 50 // number of fd we want to handle at a time
#define FD_FREE 0
#define FD_SERV 1
#define FD_CLIENT 2

#define NO_HOST 0

#define MAX(x, y) ((x > y) ? x : y) 

// typedef struct 	s_fd {
//     int     type;
//     int     host;
//     void(MasterServer::*fct_read) (int);
//     void(MasterServer::*fct_write) (int);
//     char    buf_read[BUF_SIZE + 1];
//     char    buf_write[BUF_SIZE + 1];
// } 	t_fd;


#endif