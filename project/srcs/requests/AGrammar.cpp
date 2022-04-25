#include "AGrammar.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AGrammar::AGrammar()
{
}

AGrammar::AGrammar( const AGrammar & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AGrammar::~AGrammar()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AGrammar &				AGrammar::operator=( AGrammar const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, AGrammar const & i )
{
	i.print(o);
	return o;
}



/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */