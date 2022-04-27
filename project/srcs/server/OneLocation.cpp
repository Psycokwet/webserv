#include "OneLocation.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

DIRECTIVES_MAP OneLocation::initializeDirectivesMap()
{
	DIRECTIVES_MAP map;

	map["index"] = &ALocation::addIndex;
	map["root"] = &ALocation::addRoot;
	map["autoindex"] = &ALocation::addAutoIndex;
	map["method"] = &ALocation::addMethod;
	map["client_max_body_size"] = &ALocation::addMaxSize;
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
	o << "\tAnd I have as _root = " << _root;
	o << "\tAnd I have as _autoindex = " << _autoindex;
	o << "\tI have as _method = ";
	for (std::set<std::string>::const_iterator it = _method.begin(); it != _method.end(); it++)
		o << *it << " ";
	o << "\tAnd I have as _client_max_body_size = " << _client_max_body_size;
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

AServerItem *OneLocation::addIndex(Node *node)
{
	std::cout << "OneLocation I'm trying to add a index directive from " << *node ;
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

AServerItem *OneLocation::addRoot(Node *node)
{
	std::cout << "OneLocation I'm trying to add a root directive from " << *node;
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

AServerItem *OneLocation::addAutoIndex(Node *node)
{
	std::cout << "OneLocation I'm trying to add a autoindex directive from " << *node;
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

AServerItem *OneLocation::addMethod(Node *node)
{
	std::cout << "OneLocation I'm trying to add a Method directive from " << *node ;
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

AServerItem *OneLocation::addMaxSize(Node *node)
{
	std::cout << "OneLocation I'm trying to add a client_max_body_size directive from " << *node;
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

DIRECTIVES_MAP & OneLocation::getDirectiveMap()
{
	return this->_directives_to_setter;
}
