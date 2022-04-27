#include "OneServer.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

DIRECTIVES_MAP OneServer::initializeDirectivesMap()
{
	DIRECTIVES_MAP map;
	map["location"] = &ALocation::addLocation;
	map["server_name"] = &ALocation::addServerName;
	// map["listen"] = &ALocation::addListen;

	map["index"] = &ALocation::addIndex;
	map["root"] = &ALocation::addRoot;
	map["autoindex"] = &ALocation::addAutoIndex;
	map["method"] = &ALocation::addMethod;
	map["client_max_body_size"] = &ALocation::addMaxSize;
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
	// Todo: Add default value for Listen and Location
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

OneServer::~OneServer()
{
	for (std::map< std::string, OneLocation* >::const_iterator it = this->_location.begin(); it != this->_location.end(); it++)
		delete it->second;
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::ostream &			OneServer::print( std::ostream & o) const
{
	o << "\tI'm OneServer and I have as _server_name = ";
	for (unsigned long i = 0; i < _server_name.size(); i++)
		o << _server_name[i] << " ";
	o << "\tAnd I have as _index = ";
	for (unsigned long i = 0; i < _index.size(); i++)
		o << _index[i] << " ";
	o << "\tAnd I have as _root = " << _root;
	o << "\tAnd I have as _autoindex = " << _autoindex;
	o << "\tAnd I have as _method = ";
	for (std::set<std::string>::const_iterator it = _method.begin(); it != _method.end(); it++)
		o << *it << " ";
	o << std::endl;
	for (std::map< std::string, OneLocation* >::const_iterator it = this->_location.begin(); it != this->_location.end(); it++)
		o << "\t\t" << *(it->second) << std::endl;
	o << "\tAnd I have as _client_max_body_size = " << _client_max_body_size;
	
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

AServerItem *OneServer::addServerName(Node *node)
{
	std::cout << "OneServer I'm trying to add a server_name directive from " << *node;
	
	if (this->_server_name[0].compare("") == 0 && this->_server_name.size() == 1 )
	{
		_server_name.clear();
		Node::t_inner_args_container values = node->get_inner_args();
		for (unsigned long i = 1; i < values.size(); i++)
			_server_name.push_back(values[i]);
	}
	else
		throw MultipleDeclareError();
	return this;
}

AServerItem *OneServer::addLocation(Node *node)
{
	std::cout << "OneServer I'm trying to add a location directive from " << *node;

	Node::t_inner_args_container values = node->get_inner_args();
	if(values.size() != 2) // necessary because juste after you try to read values[1]
		throw IncompleteDirective();
	OneLocation * location_value = new OneLocation();
	if (_location.find(values[1]) != _location.end())
		throw DuplicateUriError();
	_location[values[1]] =  location_value;
	return location_value;
}


AServerItem *OneServer::addIndex(Node *node)
{
	std::cout << "OneServer I'm trying to add a index directive from " << *node ;
	if (this->_index[0].compare("index.html") == 0 && this->_index.size() == 1 )
	{
		Node::t_inner_args_container values = node->get_inner_args();
		if (values.size() < 2)
			throw InvalidValueError();
		_index.clear();
		for (unsigned long i = 1; i < values.size(); i++)
			_index.push_back(values[i]);
	}
	else
		throw MultipleDeclareError();
	return this;
}

AServerItem *OneServer::addRoot(Node *node)
{
	std::cout << "OneServer I'm trying to add a root directive from " << *node;
	if (this->_root.compare("html") == 0)
	{
		_root.clear();
		Node::t_inner_args_container values = node->get_inner_args();
		if (values.size() != 2)
			throw IncompleteDirective();
		_root.assign(values[1]);
	}
	else
		throw MultipleDeclareError();
	return this;
}

AServerItem *OneServer::addAutoIndex(Node *node)
{
	std::cout << "OneLocation I'm trying to add a method directive from " << *node;
	if (this->_autoindex == false)
	{
		Node::t_inner_args_container values = node->get_inner_args();
		if (values.size() != 2)
			throw IncompleteDirective();
		if (values[1].compare("on") == 0)
			_autoindex = true;
		else if (values[1].compare("off") == 0)
			_autoindex = false;
		else
			throw InvalidValueError();
	}
	else
		throw MultipleDeclareError();
	return this;
}

AServerItem *OneServer::addMethod(Node *node)
{
	std::cout << "OneServer I'm trying to add a Method directive from " << *node ;
	if (this->_method.begin()->compare("GET") == 0 && this->_method.size() == 1 )
	{
		Node::t_inner_args_container values = node->get_inner_args();
		if (values.size() < 2)
			throw InvalidValueError();
		_method.clear();
		for (unsigned long i = 1; i < values.size(); i++)
			_method.insert(values[i]);
		for (std::set<std::string>::const_iterator it = _method.begin(); it != _method.end(); it++)
		{
			if (it->compare("GET") != 0 && it->compare("POST") != 0 && it->compare("DELETE") != 0)
				throw InvalidValueError();
		}
	}
	else
		throw MultipleDeclareError();
	return this;
}

AServerItem *OneServer::addMaxSize(Node *node)
{
	std::cout << "OneServer I'm trying to add a client_max_body_size directive from " << *node;
	if (this->_client_max_body_size == 1)
	{
		Node::t_inner_args_container values = node->get_inner_args();
		if (values.size() != 2)
			throw IncompleteDirective();
		std::stringstream degree(values[1]);
		degree >> _client_max_body_size;
	}
	else
		throw MultipleDeclareError();
	return this;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

DIRECTIVES_MAP & OneServer::getDirectiveMap()
{
	return this->_directives_to_setter;
}


/* ************************************************************************** */