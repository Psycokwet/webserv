#include "ActionForKey.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ActionForKey::ActionForKey(int min, int max, void (ConfigConsumer::*consume)(void* accumulator) const): _min_level(min), _max_level(max), _consume(consume)
{
}

ActionForKey::ActionForKey( const ActionForKey & src ): _min_level(src._min_level), _max_level(src._max_level), _consume(src._consume)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ActionForKey::~ActionForKey()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ActionForKey &				ActionForKey::operator=( ActionForKey const & rhs )
{
	if ( this != &rhs )
	{
		this->_max_level = rhs._max_level;
		this->_min_level = rhs._min_level;
		this->_consume = rhs._consume;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ActionForKey const & i )
{
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool ActionForKey::isValid(int level) const
{

	if(level > this->_min_level && level < this->_max_level)
		return true;
	return false;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */