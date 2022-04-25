#include "OneServer.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

DIRECTIVES_MAP OneServer::initializeDirectivesMap()
{
    DIRECTIVES_MAP map;
	map["listen"] = &OneServer::addListen;
	map["server_name"] = &OneServer::addServerName;
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
	// ! Get corresponding function to function pointer named directiveConsumer, based on the name of the directive
	AServerItem *(OneServer::*directiveConsumer)(Node*) = OneServer::_directives_to_setter[*(inner_args.begin())];
	if(!directiveConsumer)
		return NULL; // Or throw, whichever feels best
	// ! Call and Run the corresponding function, but how to change/add value to the same OneServer???
	// ! The function does some job and change inside value of OneServer object. (return this)
	return (this->*directiveConsumer)(node); 
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

OneServer::OneServer() //Todo: put default value to each directive
{
    _server_name.push_back("");
    // std::pair<std::string, int> default_addr1("*", 80);
    // std::pair<std::string, int> default_addr2("*", 8000);
    // _listen.push_back(default_addr1);
    // _listen.push_back(default_addr2);

    // _root = "html";

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

AServerItem *OneServer::addServerName(Node *node)
{
	std::cout << "I'm trying to add a server_name directive from " << *node << std::endl;
	if (this->_server_name[0].compare("") == 0 && this->_server_name.size() == 1 )
	{
		std::cout << "Add server name for the first time from: " << *node << std::endl;
		_server_name.empty();
		Node::t_inner_args_container values = node->get_inner_args();
		std::cout << "_server_name = ";
		for (unsigned long i = 1; i < values.size(); i++)
		{
			_server_name.push_back(values[i]);
			std::cout << _server_name[i] << " ";
		}
		std::cout << std::endl;
		
	}
	else
		// std::cout << "... trying to add server name more than one time from: " << *node << " ==> ERROR"<<std::endl;
		throw MultipleDeclareError();
	return this;

}



/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */