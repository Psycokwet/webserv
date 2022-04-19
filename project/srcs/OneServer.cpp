#include "OneServer.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

DIRECTIVES_MAP OneServer::initializeDirectivesMap()
{
    DIRECTIVES_MAP map;
	map["listen"] = &OneServer::addListen;
    return map;
}

DIRECTIVES_MAP OneServer::_directives_to_setter = OneServer::initializeDirectivesMap();


AServerItem *OneServer::consume(Node *node)
{
	if(!node)
		return NULL; // Or throw, whichever feels best
	Node::t_inner_args_container &inner_args = node->getInnerArgs(); // ! get all elements of server key (get all servers)
	if (inner_args.size() == 0)
		return NULL; // Or throw, whichever feels best
	AServerItem *(OneServer::*directiveConsumer)(Node*) = OneServer::_directives_to_setter[*(inner_args.begin())];
	if(!directiveConsumer)
		return NULL; // Or throw, whichever feels best
	return (this->*directiveConsumer)(node);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

OneServer::OneServer() //Todo: put default value to each directive
    // _root(NULL),
    // _server_name(NULL),
    // _autoindex(NULL);
    // _location(NULL)
{
    // std::pair<std::string, int> default_addr1("*", 80);
    // std::pair<std::string, int> default_addr2("*", 8000);
    // _listen.push_back(default_addr1);
    // _listen.push_back(default_addr2);

    // _root = "html";
    _server_name.push_back("");

}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

OneServer::~OneServer()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


std::ostream &			OneServer::print( std::ostream & o) const
{
	o << "I'm OneServer";
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/




AServerItem *OneServer::addListen(Node *node)
{
	std::cout << "I'm trying to add a listen directive from " << *node << std::endl;
	//do something to add listener
	(void)node;
	return this;
}



/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */