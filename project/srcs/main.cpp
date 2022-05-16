/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-nguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#             */
/*   Updated: 2022/05/16 11:37:24 by thi-nguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return 1;
	std::cout <<"Print result " << *ms << std::endl;

	if (ms->build() != 0) // ! Better throw something?
	{
		std::cout << "Error: Can not setup MasterServer" << std::endl;
		return 1;
	}
	// ms->run();
	delete ms;
	return 0;
}