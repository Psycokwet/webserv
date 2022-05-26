#ifndef DELETE_HPP
# define DELETE_HPP
# include <cstring>
# include <string.h>
# include <ctype.h>
# include <algorithm>
# include <map>
# include <list>
# include <vector>

template <typename T> 
void util_delete(std::ostream &o, std::map<std::string, T> container)
{
	for (typename std::map<std::string,T>::iterator it = container.begin(); it != container.end(); it++)
		delete it;
	container.clear();
}

template <typename T> 
void util_delete(std::ostream &o, std::list<T> container)
{
	for (typename std::list<T>::iterator it = container.begin(); it != container.end(); it++)
		delete it;
	container.clear();
}

template <typename T> 
void util_delete(std::ostream &o, std::vector<T> container)
{
	for (int i = 0; container.size(); i++)
		delete container[i];
	container.clear();
}

#endif