/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#             */
/*   Updated: 2022/03/19 22:14:43 by scarboni         ###   ########.fr       */
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
# include <sys/epoll.h>
//epoll epoll_create epoll_ctl epoll_wait
// # include <sys/kqueue.h> not working for some reasons on my vm, should try at school
//kqueue kevent
# include <sys/socket.h>
//socket accept listen send recv bind connect setsockopt getsockname
# include <netinet/in.h>
//inet_addr 
#include <fcntl.h>
//fcntl

# include <iostream>
#endif