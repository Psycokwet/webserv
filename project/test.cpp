#include <iostream>
#include <string>
#include <map>

#include <sstream>

#include <iostream>
#include <limits> // numberic_limits
#include <cstring>
#include <set>
#include "Server.hpp"

Server	*gServer = NULL;

int main ()
{
  gServer = new Server();
	gServer->SetUp();
	gServer->Run();
} 