#include "ActionForKey.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ActionForKey::ActionForKey(int min, int max, std::vector<std::string> parents, AServerItem *(consume)(Node *node, AServerItem *currentServerItem))
	: _min_level(min), _max_level(max), _parents(parents), _consume(consume)
{
}

ActionForKey::ActionForKey(const ActionForKey &src)
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

	if (!(level > this->_min_level && level < this->_max_level)) // Better to fail early than have a lot of {} imbrications
		return false;
	for (unsigned int i = 0; i < this->_parents.size(); i++)
	{
		if ((*parent).compare(this->_parents[i]) != 0)
			return false;
	}
	return true;
}

AServerItem *ActionForKey::consume(Node *node, AServerItem *currentServerItem) const
{
	if (!this->_consume)
		throw ConsumerNotDefined();
	return this->_consume(node, currentServerItem);
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