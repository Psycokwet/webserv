/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#             */
/*   Updated: 2022/03/21 18:02:31 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.h"
#include "Config.hpp"

int		main(int ac, char *av[])
{
	if (ac != 2)
	{
		std::cout << "Usage : ./webserv [configuration file]" << std::endl;
		return 0;
	}

	Config *config = Config::factory(av[1]);
	std::cout << *config << std::endl;
	if(!config)
		return 0;
	delete config;
	return 0;
}