#include "toArray.hpp"

char **stringToArray(std::vector<std::string> argVector)
{
    char **return_array = new char*[argVector.size() + 1];
    unsigned long i;
    for (i = 0; i < argVector.size(); i++)
    {
        return_array[i] = new char[argVector[i].length() + 1];
        std::strcpy(return_array[i], argVector[i].c_str());
    }
    return_array[i] = NULL;
    return return_array;
}

char**  mapToArray(std::map<std::string, std::string> envVars)
{
    char **return_array = new char*[envVars.size() + 1];
    int k = 0;
    std::map<std::string, std::string>::const_iterator i;
    for (i = envVars.begin(); i != envVars.end(); i++)
    {
        std::string item = i->first + "=" + i->second;

        return_array[k] = new char[item.length() + 1];
        std::strcpy(return_array[k], item.c_str());
        k++;
    }
    return_array[k] = NULL;
    return return_array;
}

