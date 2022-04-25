#include "OneLocation.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

LOCATION_DIRECTIVES_MAP OneLocation::initializeDirectivesMap()
{
    LOCATION_DIRECTIVES_MAP map;
	map["index"] = &OneLocation::addIndex;
    return map;
}

LOCATION_DIRECTIVES_MAP OneLocation::_directives_to_setter = OneLocation::initializeDirectivesMap();


AServerItem *OneLocation::consume(Node *node)
{
	if(!node)
		return NULL; // Or throw, whichever feels best
	Node::t_inner_args_container &inner_args = node->getInnerArgs(); // ! get all elements of server key (get all servers)
	if (inner_args.size() == 0)
		return NULL; // Or throw, whichever feels best
	// ! Get corresponding function to function pointer named directiveConsumer, based on the name of the directive
	AServerItem *(OneLocation::*directiveConsumer)(Node*) = OneLocation::_directives_to_setter[*(inner_args.begin())];
	if(!directiveConsumer)
		return NULL; // Or throw, whichever feels best
	// ! Call and Run the corresponding function, but how to change/add value to the same OneServer???
	// ! The function does some job and change inside value of OneServer object. (return this)
	return (this->*directiveConsumer)(node); 
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

OneLocation::OneLocation() //Todo: put default value to each directive
{

}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

OneLocation::~OneLocation()
{
}

std::ostream &			OneLocation::print( std::ostream & o) const
{
	o << "I'm OneLocation";
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

AServerItem *OneLocation::addIndex(Node *node)
{
	std::cout << "I'm trying to add a index directive to Location parent, from " << *node << std::endl;
	(void)node;
	return this;
}