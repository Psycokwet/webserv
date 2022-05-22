#include <iostream>
#include "CgiHandler.hpp"

class CgiHandler;

int main()
{
    CgiHandler my_object;

    std::string result;
    std::string scriptName;

    result = my_object.executeCgi(scriptName);
    std::cout << result << std::endl;
    return (0);

}
