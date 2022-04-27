#include "OneLocation.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

DIRECTIVES_MAP OneLocation::initializeDirectivesMap()
{
	DIRECTIVES_MAP map;
	map["listen"] = &ALocation::addListen;
	map["index"] = &ALocation::addIndex;
    return map;
}

DIRECTIVES_MAP OneLocation::_directives_to_setter = OneLocation::initializeDirectivesMap();

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

OneLocation::OneLocation()
{
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
	o << "I'm OneLocation and I have as _index = ";
	for (unsigned long i = 0; i < _index.size(); i++)
		o << _index[i] << " ";
	return o;
}

AServerItem *OneLocation::addIndex(Node *node)
{
	std::cout << "Add index for the first time from: " << *node;
	_index.clear();
	Node::t_inner_args_container values = node->get_inner_args();
	for (unsigned long i = 1; i < values.size(); i++)
	{
		_index.push_back(values[i]);
		// std::cout << values[i] << " ";
		// std::cout << _index[i] << " ";
	}

	std::cout << "_index = ";
	for (unsigned long i = 0; i < _index.size(); i++)
		std::cout << _index[i] << " ";
	std::cout << std::endl;
	return this;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

DIRECTIVES_MAP & OneLocation::getDirectiveMap()
{
	return this->_directives_to_setter;
}
