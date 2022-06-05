#include "compareContainers.hpp"

bool compareCont(std::list<std::string> cont1, std::list<std::string> cont2)
{
	if (cont1.size() != cont2.size())
		return false;
	std::list<std::string>::const_iterator it2 = cont2.begin();
	for (std::list<std::string>::const_iterator it1 = cont1.begin(); it1 != cont1.end(); it1++, it2++)
	{
		if (*it1 != *it2)
			return false;
	}
	return true;
}

bool compareCont(std::vector<std::string> cont1, std::vector<std::string> cont2)
{
	if (cont1.size() != cont2.size())
		return false;
	std::vector<std::string>::const_iterator it2 = cont2.begin();
	for (std::vector<std::string>::const_iterator it1 = cont1.begin(); it1 != cont1.end(); it1++, it2++)
	{
		if (*it1 != *it2)
			return false;
	}
	return true;
}

bool compareCont(std::set<std::string> cont1, std::set<std::string> cont2)
{
	if (cont1.size() != cont2.size())
		return false;
	std::set<std::string>::const_iterator it2 = cont2.begin();
	for (std::set<std::string>::const_iterator it1 = cont1.begin(); it1 != cont1.end(); it1++, it2++)
	{
		if (*it1 != *it2)
			return false;
	}
	return true;
}

/*
** -------------------------------- COMPARE ALLOWING EXTERN WILDCARD --------------------------------
*/
bool compareContWC(std::list<std::string> cont1, std::list<std::string> cont2, std::string wildcard)
{
	if (cont1.size() != cont2.size())
		return false;
	std::list<std::string>::const_iterator it2 = cont2.begin();
	for (std::list<std::string>::const_iterator it1 = cont1.begin(); it1 != cont1.end(); it1++, it2++)
	{
		if (*it1 != *it2 && *it1 != wildcard && *it2 != wildcard)
			return false;
	}
	return true;
}

bool compareContWC(std::vector<std::string> cont1, std::vector<std::string> cont2, std::string wildcard)
{
	if (cont1.size() != cont2.size())
		return false;
	std::vector<std::string>::const_iterator it2 = cont2.begin();
	for (std::vector<std::string>::const_iterator it1 = cont1.begin(); it1 != cont1.end(); it1++, it2++)
	{
		if (*it1 != *it2 && *it1 != wildcard && *it2 != wildcard)
			return false;
	}
	return true;
}

bool compareContWC(std::set<std::string> cont1, std::set<std::string> cont2, std::string wildcard)
{
	if (cont1.size() != cont2.size())
		return false;
	std::set<std::string>::const_iterator it2 = cont2.begin();
	for (std::set<std::string>::const_iterator it1 = cont1.begin(); it1 != cont1.end(); it1++, it2++)
	{
		if (*it1 != *it2 && *it1 != wildcard && *it2 != wildcard)
			return false;
	}
	return true;
}

/*
** -------------------------------- COMPARE ALLOWING WILDCARD --------------------------------
*/
bool compareContWC(std::list<std::string> cont1, std::list<std::string> cont2)
{
	return compareContWC(cont1, cont2, WILDCARD);
}

bool compareContWC(std::vector<std::string> cont1, std::vector<std::string> cont2)
{
	return compareContWC(cont1, cont2, WILDCARD);
}

bool compareContWC(std::set<std::string> cont1, std::set<std::string> cont2)
{
	return compareContWC(cont1, cont2, WILDCARD);
}
