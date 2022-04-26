#include "OneServer.hpp"
#include "OneLocation.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

DIRECTIVES_MAP OneServer::initializeDirectivesMap()
{
    DIRECTIVES_MAP map;
	map["listen"] = &OneServer::addListen;
	map["server_name"] = &OneServer::addServerName;
	map["location"] = &OneServer::addLocation;
	map["index"] = &OneServer::addIndex;
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
	_index.push_back("index.html");

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
	std::cout << "I'm trying to add a listen directive from " << *node;
	//do something to add listener
	(void)node;
	return this;
}

AServerItem *OneServer::addServerName(Node *node)
{
	std::cout << "I'm trying to add a server_name directive from " << *node;
	if (this->_server_name[0].compare("") == 0 && this->_server_name.size() == 1 )
	{
		std::cout << "Add server name for the first time from: " << *node;
		_server_name.clear();
		Node::t_inner_args_container values = node->get_inner_args();
		for (unsigned long i = 1; i < values.size(); i++)
			_server_name.push_back(values[i]);

		std::cout << "_server_name = ";
		for (unsigned long i = 0; i < _server_name.size(); i++)
			std::cout << _server_name[i] << " ";
		std::cout << std::endl;
		
	}
	else
		// std::cout << "... trying to add server name more than one time from: " << *node << " ==> ERROR"<<std::endl;
		throw MultipleDeclareError();
	return this;
}

AServerItem *OneServer::addLocation(Node *node)
{
	std::cout << "I'm trying to add a location directive from " << *node;

	Node::t_inner_args_container values = node->get_inner_args();
	AServerItem * location_value = new OneLocation;

	if (_location.find(values[1]) != _location.end())
		throw DuplicateUriError();

	_location[values[1]] =  *dynamic_cast<OneLocation*>(location_value);

	// std::cout << "___Key of location_map are: ";
	// for (std::map<std::string, OneLocation>::iterator it = _location.begin(); it != _location.end(); it++)
	// 	std::cout << it->first << "     ";
	// std::cout << std::endl;

	return location_value;
}


AServerItem *OneServer::addIndex(Node *node)
{
	std::cout << "I'm trying to add a index directive from " << *node;
	if (this->_index[0].compare("index.html") == 0 && this->_server_name.size() == 1 )
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
		
	}
	else
	{
		std::cout << "_index = ";
		for (unsigned long i = 0; i < _index.size(); i++)
			std::cout << _index[i] << " ";
		std::cout << std::endl;
		throw MultipleDeclareError();
	}
		// std::cout << "... trying to add server name more than one time from: " << *node << " ==> ERROR"<<std::endl;
	return this;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */