#ifndef CONFIGDATASTRUCTURES_HPP
# define CONFIGDATASTRUCTURESHPP

#include <string>
#include <vector>

typedef struct 	s_listen {
	std::string 		host;
	int					port;
	std::string 		default_server;
} 				t_listen;

typedef struct 	s_error_page {
	public:
		std::vector<int>    errorCodes; 
		std::string         uri;		
	
		// s_error_page() {
		// 	errorCodes.push_back(NULL);
		// 	uri = "";
		// };
} 				t_error_page;

#endif /* ************************************************** CONFIGDATASTRUCTURES_HPP */