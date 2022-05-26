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
std::ostream & print_cont(std::ostream &o, std::map<std::string, T>& container, std::string end = "\n", std::string start = "", std::string sep = "")
{
	for (typename std::map<std::string,T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << start << it->first << sep << it->second << end;
	}
	return o;
}

template <typename T> 
std::ostream & print_cont(std::ostream &o, std::list<T>& container, std::string end = "\n", std::string start = "")
{
	for (typename std::list<T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << start << *it << end;
	}
	return o;
}

template <typename T> 
std::ostream & print_cont(std::ostream &o, std::vector<T>& container, std::string end = "\n", std::string start = "")
{
	for(typename std::vector<T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << start << *it << end;
	}
	return o;
}

/*
** -------------------------------- CONST CONTAINERS --------------------------------
*/

template <typename T> 
std::ostream & print_cont(std::ostream &o, const std::map<std::string, T>& container, std::string end = "\n", std::string start = "", std::string sep = "")
{
	for (typename std::map<std::string,T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << start << it->first << sep << it->second << end;
	}
	return o;
}

template <typename T> 
std::ostream & print_cont(std::ostream &o, const std::list<T>& container, std::string end = "\n", std::string start = "")
{
	for (typename std::list<T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << start << *it << end;
	}
	return o;
}

template <typename T> 
std::ostream & print_cont(std::ostream &o, const std::vector<T>& container, std::string end = "\n", std::string start = "")
{
	for(typename std::vector<T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << start << *it << end;
	}
	return o;
}

/*
** -------------------------------- POINTER CONTAINERS --------------------------------
*/

template <typename T> 
std::ostream & print_ptr(std::ostream &o, std::map<std::string, T>& container, std::string end = "\n", std::string start = "", std::string sep = "")
{
	for (typename std::map<std::string,T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << start << it->first << sep << *it->second << end;
	}
	return o;
}

template <typename T> 
std::ostream & print_ptr(std::ostream &o, std::list<T>& container, std::string end = "\n", std::string start = "")
{
	for (typename std::list<T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << start << **it << end;
	}
	return o;
}

template <typename T> 
std::ostream & print_ptr(std::ostream &o, std::vector<T>& container, std::string end = "\n", std::string start = "")
{
	for (int i = 0; container.size(); i++)
	{
		o << start << *container[i] << end;
	}
	return o;
}

/*
** -------------------------------- CONST POINTER CONTAINERS --------------------------------
*/


template <typename T> 
std::ostream & print_ptr(std::ostream &o, const std::map<std::string, T>& container, std::string end = "\n", std::string start = "", std::string sep = "")
{
	for (typename std::map<std::string,T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << start << it->first << sep << *it->second << end;
	}
	return o;
}

template <typename T> 
std::ostream & print_ptr(std::ostream &o, const std::list<T>& container, std::string end = "\n", std::string start = "")
{
	for (typename std::list<T>::const_iterator it = container.begin(); it != container.end(); it++)
	{
		o << start << **it << end;
	}
	return o;
}

template <typename T> 
std::ostream & print_ptr(std::ostream &o, const std::vector<T>& container, std::string end = "\n", std::string start = "")
{
	for (int i = 0; container.size(); i++)
	{
		o << start << *container[i] << end;
	}
	return o;
}

#endif