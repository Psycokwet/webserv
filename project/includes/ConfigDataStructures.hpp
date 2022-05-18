#ifndef CONFIGDATASTRUCTURES_HPP
# define CONFIGDATASTRUCTURES_HPP

#include <string>
#include <vector>
#include "webserv.h"

typedef struct 	s_listen {
	uint32_t 		_address;
	int				_port;
	std::string 	_default_server;

	s_listen(uint32_t address = LOCALHOST, int port=80, std::string default_server="")
	{
		_address = address;
		_port = port;
		_default_server = default_server;
	}
} 				t_listen;

typedef struct 	s_error_page {
	std::vector<int>    errorCodes; 
	std::string         uri;

	s_error_page(std::string a_uri="")
	{
		uri = a_uri;
	}		
} 				t_error_page;

#endif /* ************************************************** CONFIGDATASTRUCTURES_HPP */