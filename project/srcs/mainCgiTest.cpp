#include "CgiHandler.hpp"
#include <iostream>


int main()
{
    CgiHandler my_cgi;
    std::string body;
    std::string fileName = "../../cgi_test.php";

    body = my_cgi.executeCgi(fileName);
    
    std::cout << body << std::endl;
}