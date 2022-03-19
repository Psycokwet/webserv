/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#             */
/*   Updated: 2022/03/19 22:15:00 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.h"

int		main(int ac, char *av[])
{
	if (ac != 2)
	{
		std::cout << "Usage : ./webserv [configuration file]" << std::endl;
		return 0;
	}
	(void)av;
	return 0;
}