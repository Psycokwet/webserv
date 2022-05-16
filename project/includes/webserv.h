/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idamouttou <idamouttou@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#             */
/*   Updated: 2022/05/05 14:08:58 by idamouttou       ###   ########.fr       */
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
//Bytes endian conversion
# define BYTE_0 0xff000000
# define BYTE_1 0x00ff0000
# define BYTE_2 0x0000ff00
# define BYTE_3 0x000000ff

# define ft_htonl ft_ntohl
# define ft_htons ft_ntohs
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

//ntoh / hton
unsigned short				ft_ntohs(unsigned short i);
unsigned short				ft_htons(unsigned short i);

unsigned int				ft_htonl(unsigned int i);
unsigned int                ft_ntohl(unsigned int i);




# include <iostream>
#endif