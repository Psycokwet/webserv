#include "ActionForKey.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ActionForKey::ActionForKey(int min, int max): _min_level(min), _max_level(max)
{
}

ActionForKey::ActionForKey( const ActionForKey & src ): _min_level(src._min_level), _max_level(src._max_level)
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
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ActionForKey const & i )
{
	(void)i;
	o << "min = " << i.getMinLevel() << "max = " << i.getMaxLevel();
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


int ActionForKey::getMinLevel() const
{
	return this->_min_level;
}

int ActionForKey::getMaxLevel() const
{
	return this->_max_level;
}

/* ************************************************************************** */