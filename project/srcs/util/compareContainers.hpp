#ifndef CONTAINER_TO_HPP
#define CONTAINER_TO_HPP
#include <cstring>
#include <string.h>
#include <ctype.h>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <sstream>

bool compareCont(std::list<std::string> cont1, std::list<std::string> cont2);
bool compareCont(std::vector<std::string> cont1, std::vector<std::string> cont2);
bool compareCont(std::set<std::string> cont1, std::set<std::string> cont2);

/*
** -------------------------------- COMPARE ALLOWING EXTERN WILDCARD --------------------------------
*/
bool compareContWC(std::list<std::string> cont1, std::list<std::string> cont2, std::string wildcard);
bool compareContWC(std::vector<std::string> cont1, std::vector<std::string> cont2, std::string wildcard);
bool compareContWC(std::set<std::string> cont1, std::set<std::string> cont2, std::string wildcard);

/*
** -------------------------------- COMPARE ALLOWING WILDCARD --------------------------------
*/
#define WILDCARD "*"
bool compareContWC(std::list<std::string> cont1, std::list<std::string> cont2);
bool compareContWC(std::vector<std::string> cont1, std::vector<std::string> cont2);
bool compareContWC(std::set<std::string> cont1, std::set<std::string> cont2);

#endif