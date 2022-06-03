#ifndef TOARRAY_HPP
#define TOARRAY_HPP

#include <vector>
#include <string>
#include <map>
#include <cstring>

char **stringToArray(std::vector<std::string> argVector);
char**  mapToArray(std::map<std::string, std::string> envVars);

#endif