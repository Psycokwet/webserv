/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainParserTest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#             */
/*   Updated: 2022/05/03 18:42:09 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.h"
#include "requests/GrammarParser.hpp"
# include <ctype.h>
# include <iostream>
# include <sstream>
# include <cstring>
# include <list>
# include <map>
# include <vector>
# include <algorithm>
# include <fstream>

int		main(int ac, char **av)
{
	(void)av;
	if (ac != 2)
	{
		std::cout << "I'm only a request parser for test, what do you expect by not giving me any file name to read from?" << std::endl;
		return 0;
	}
	GrammarParser *gp = GrammarParser::build(GRAMMAR_FILE);
	if(!gp)
		return 0;
	// std::cout << *gp << std::endl;
	std::cout << "Grammar parser is valid" << std::endl;

	std::ifstream	ifs(av[1]);
	std::string		tmp_line = "";
	std::stringstream ss;
	while(std::getline(ifs, tmp_line))
	{
		gp->feed(tmp_line + "\n\r");
		std::cout << "I've read :" << tmp_line<< std::endl;
	}
	std::cout << GetString(gp->parse()) << tmp_line<< std::endl;
	delete gp;
	return 0;
}