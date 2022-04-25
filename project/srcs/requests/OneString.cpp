#include "OneString.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

OneString::OneString(std::string value) : _value(value)
{
}

OneString::OneString( const OneString & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

OneString::~OneString()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

OneString &				OneString::operator=( OneString const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			OneString::print( std::ostream & o) const
{
	o << "I'm OneString";
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */