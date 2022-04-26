#include "ActionForKey.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ActionForKey::ActionForKey(int min, int max, std::vector<std::string> parents, AServerItem *(consume)(Node *node, AServerItem* currentServerItem, bool isLocation))
: _min_level(min), _max_level(max), _parents(parents), _consume(consume)
{
}

ActionForKey::ActionForKey( const ActionForKey & src )
: _min_level(src._min_level), _max_level(src._max_level), _parents(src._parents), _consume(src._consume)
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
		this->_parents = rhs._parents;
		this->_consume = rhs._consume;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ActionForKey const & i )
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
	return false;
}

AServerItem *ActionForKey::consume(Node *node, AServerItem* currentServerItem, bool isLocation) const
{
	if(!this->_consume)
		throw new ConsumerNotDefined();
	return this->_consume(node, currentServerItem, isLocation); // ! return a function coresponded to that directive, and execute that function?
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