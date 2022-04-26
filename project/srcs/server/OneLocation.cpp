#include "OneLocation.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

OneLocation::OneLocation()
{
	std::cout << "HELLO ONE LOCATION HERE ----------------------------------------------\n" << this << std::endl << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

OneLocation::~OneLocation()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::ostream &			OneLocation::print( std::ostream & o) const
{
	o << "I'm OneLocation";
	return o;
}

