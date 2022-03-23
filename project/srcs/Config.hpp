#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <ctype.h>
# include <iostream>
# include <sstream>
# include <cstring>
# include <list>
# include <map>
# include <algorithm>
# include <fstream>
# include "Node.hpp"
class Config
{

	public:

		~Config();
		static Config *factory(std::string input_file);
		
        class IllegalConstructorException : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };

	private:
		static Node _default_node;
		Config(std::string input_file = "", Node &first_node = _default_node);
		Config( Config const & src );
		Config &		operator=( Config const & rhs );

		std::string _input_file;
		Node &_first_node;

		// static int splitAddToNode(std::string &s, Node *current_node);
		// int parseObject(std::ifstream ifs, std::string tmp_line, Node *current_node);
};

std::ostream &			operator<<( std::ostream & o, Config const & i );

#endif /* ********************************************************** CONFIG_H */