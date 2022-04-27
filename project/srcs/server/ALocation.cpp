#include "ALocation.hpp"
#include "OneLocation.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

AServerItem *ALocation::consume(Node *node)
{
	if(!node)
		return NULL; // Or throw, whichever feels best
	Node::t_inner_args_container &inner_args = node->getInnerArgs(); // ! get all elements of server key (get all servers)
	if (inner_args.size() == 0)
		return NULL; // Or throw, whichever feels best
	// ! Get corresponding function to function pointer named directiveConsumer, based on the name of the directive
	AServerItem *(ALocation::*directiveConsumer)(Node*) = this->getDirectiveMap()[*(inner_args.begin())];
	if(!directiveConsumer)
		return NULL; // Or throw, whichever feels best
	// ! Call and Run the corresponding function, but how to change/add value to the same ALocation???
	// ! The function does some job and change inside value of ALocation object. (return this)
	return (this->*directiveConsumer)(node); 
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ALocation::ALocation() //Todo: put default value to each directive
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ALocation::~ALocation()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


std::ostream &			ALocation::print( std::ostream & o) const
{
	(void)o;
	throw new IncompleteTypeError();
}
/*
** --------------------------------- METHODS ----------------------------------
*/

AServerItem *ALocation::addListen(Node *node)
{
	std::cout << "I'm trying to add a listen directive from " << *node;
	//do something to add listener
	(void)node;
	return this;
}


AServerItem *ALocation::addServerName(Node *node)
{
	(void)node;
	throw new IncompleteTypeError();
}

AServerItem *ALocation::addIndex(Node *node)
{
	(void)node;
	throw new IncompleteTypeError();
}

AServerItem *ALocation::addLocation(Node *node)
{
	(void)node;
	throw new IncompleteTypeError();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

DIRECTIVES_MAP & ALocation::getDirectiveMap()
{
	throw new IncompleteTypeError();
}

/* ************************************************************************** */