/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#             */
/*   Updated: 2022/04/19 09:14:56 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.h"
#include "config/ConfigConsumer.hpp"

#define DEFAULT_CONFIG_PATH "./config_files/default.conf"

int		main(int ac, char *av[])
{
	std::string config_path;

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

	MasterServer *ms = ConfigConsumer::validateEntry(config_path);
	if(!ms)
		return 0;
	std::cout <<"Print result " << *ms << std::endl;

	delete ms;
	return 0;
}