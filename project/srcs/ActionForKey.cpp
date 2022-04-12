#include "ActionForKey.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ActionForKey::ActionForKey(int min, int max, std::string parents1, std::string parents2)
: _min_level(min), _max_level(max), _parents1(parents1), _parents2(parents2)
{
}

ActionForKey::ActionForKey( const ActionForKey & src )
: _min_level(src._min_level), _max_level(src._max_level),
	_parents1(src._parents1), _parents2(src._parents2)
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
		this->_parents1 = rhs._parents1;
		this->_parents2 = rhs._parents2;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ActionForKey const & i )
{
	(void)i;
	o << "min = " << i.getMinLevel() << "max = " << i.getMaxLevel();
	o << "parents 1 = " << i.getParents1() << " parent2 = " << i.getParents2() << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool ActionForKey::isValid(int level, std::string parents) const
{

	if(level > this->_min_level && level < this->_max_level)
	{
		if (parents.compare(this->_parents1) == 0)
			return true;
		else if (parents.compare(this->_parents2) == 0)
			return true;
		else
			return false;
	}
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

std::string ActionForKey::getParents1() const
{
	return this->_parents1;
}

std::string ActionForKey::getParents2() const
{
	return this->_parents2;
}

/* ************************************************************************** */