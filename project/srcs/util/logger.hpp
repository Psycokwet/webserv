#ifndef LOGGER_HPP
# define LOGGER_HPP
# include <cstring>
# include <string.h>
# include <ctype.h>
# include <algorithm>
# include <fstream>
# include <iostream>
# include <sstream>
# include <ios>
# include <string>

void logger(std::string file_name, std::string log);

template <typename T> 
std::string streamFunctionToString(std::ostream &(T::*print)(std::ostream &) const, const T *obj)
{
	std::stringstream ss;
	(obj->*print)(ss);
	return ss.str();
}

#endif