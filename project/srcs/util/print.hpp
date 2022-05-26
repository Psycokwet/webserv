#ifndef PRINT_HPP
# define PRINT_HPP
# include <cstring>
# include <string.h>
# include <ctype.h>
# include <algorithm>
# include <map>
# include <list>
# include <vector>

template <typename T> 
std::ostream & print(std::ostream &o, std::map<std::string, T>& container)
{
	for (typename std::map<std::string,T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << "[" << it->first << ":" << it->second << "]" << std::endl;
	}
	return o;
}

template <typename T> 
std::ostream & print(std::ostream &o, std::list<T>& container)
{
	for (typename std::list<T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << "[" << *it << "]" << std::endl;
	}
	return o;
}

template <typename T> 
std::ostream & print(std::ostream &o, std::vector<T>& container)
{
	for (int i = 0; container.size(); i++)
	{
		o << "[" << container[i] << "]" << std::endl;
	}
	return o;
}

/*
** -------------------------------- POINTER CONTAINERS --------------------------------
*/

template <typename T> 
std::ostream & print_ptr(std::ostream &o, const std::map<std::string, T>& container)
{
	for (typename std::map<std::string,T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << "[" << it->first << ":" << *it->second << "]" << std::endl;
	}
	return o;
}

template <typename T> 
std::ostream & print_ptr(std::ostream &o, const std::list<T>& container)
{
	for (typename std::list<T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << "[" << **it << "]" << std::endl;
	}
	return o;
}

template <typename T> 
std::ostream & print_ptr(std::ostream &o, const std::vector<T>& container)
{
	for (int i = 0; container.size(); i++)
	{
		o << "[" << *container[i] << "]" << std::endl;
	}
	return o;
}



#endif