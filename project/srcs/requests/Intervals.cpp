#include "Intervals.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Intervals::Intervals(int start, int end) : _start(start), _end(end)
{
}

Intervals::Intervals( const Intervals & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Intervals::~Intervals()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Intervals &				Intervals::operator=( Intervals const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			Intervals::print( std::ostream & o) const
{
	o << "I'm Intervals";
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */