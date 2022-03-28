/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#             */
/*   Updated: 2022/03/25 09:02:41 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.h"
#include "Node.hpp"
#include "MasterServer.hpp"

#define DEFAULT_CONFIG_PATH "./config_files/default.conf"

int		main(int ac, char *av[])
{
	std::string config_path;
	MasterServer master_server;

	if (ac > 2)
	{
		std::cout << "Usage : ./webserv [configuration file]" << std::endl;
		return 0;
	}
	if (ac < 2)
	{
		config_path = DEFAULT_CONFIG_PATH;
		std::cout << "Taking default configuration file" << std::endl;
	}
	else config_path = av[1];

	master_server.parseConfig(config_path);
	if (master_server.setup() == -1)
		return (1);




	// Node *firstNode = Node::digestConfigurationFile(config_path);
	// if( !firstNode)
	// {
	// 	return 0;
	// }
	// you can get all servers objects like that :
	// Node::t_node_list servers = firstNode->getChildrenByFirstName("server");
	// for(Node::t_node_list ::const_iterator it = servers.begin(); it != servers.end(); it++)
	// {
	// 	Node::t_node_list listen_sub_first_server = (*servers.begin())->getChildrenByFirstName("index");
	// 	for(Node::t_node_list ::const_iterator it2 = listen_sub_first_server.begin(); it2 != listen_sub_first_server.end(); it2++)
	// 		std::cout << **it2 << std::endl; //two * because the list contain pointers to nodes
		
	// }
	// delete firstNode;
	return 0;
}