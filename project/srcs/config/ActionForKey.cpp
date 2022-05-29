#include "ActionForKey.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ActionForKey::ActionForKey(int min, int max, std::vector<std::string> parents)
: _min_level(min), _max_level(max), _parents(parents)
{
}

ActionForKey::ActionForKey( const ActionForKey & src )
: _min_level(src._min_level), _max_level(src._max_level), _parents(src._parents)
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

ActionForKey &ActionForKey::operator=(ActionForKey const &rhs)
{
	if (this != &rhs)
	{
		this->_max_level = rhs._max_level;
		this->_min_level = rhs._min_level;
		this->_parents = rhs._parents;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &o, ActionForKey const &i)
{
	(void)i;
	o << "min = " << i.getMinLevel() << "max = " << i.getMaxLevel();
	o << "parents = ";
	for (unsigned int index = 0; index < i.getParents().size(); index++)
		o << i.getParents()[index] << std::endl;
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool ActionForKey::isValid(int level, std::string *parent) const
{
	if(!(level > this->_min_level && level < this->_max_level)) // Better to fail early than have a lot of {} imbrications
		return false;
	// ! Directive has only one parent, check only one valid parent in the set is enough
	for (unsigned int i = 0; i < this->_parents.size(); i++)
	{
		if ((*parent).compare(this->_parents[i]) == 0)
			return true;
	}
	if(this->_parents.size() == 0 && parent == NULL)
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

std::vector<std::string> ActionForKey::getParents() const
{
	return this->_parents;
}


/* ************************************************************************** */