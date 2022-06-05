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
	map["error_page"] = &ALocation::addErrorPage;
	map["cgi"] = &ALocation::addCgi;
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
	o << "I'm OneLocation and I have as \n\t\t\t_index = ";
	for (unsigned long i = 0; i < _index.size(); i++)
		o << _index[i] << " ";
	o << "\n\t\t\t_root = " << _root;
	o << "\n\t\t\t_autoindex = " << _autoindex;

	o << "\n\t\t\t_method = ";
	for (std::set<std::string>::const_iterator it = _method.begin(); it != _method.end(); it++)
		o << *it << " ";

	o << "\n\t\t\t_client_max_body_size = " << _client_max_body_size;

	o << "\n\t\t\t_cgi = ";
	for (unsigned long i = 0; i < _cgi.size(); i++)
		o << _cgi[i] << " ";

	o << "\n\t\t\t_error_page: with codes = ";
	for (unsigned long i = 0; i < _error_page.errorCodes.size(); i++)
		o << _error_page.errorCodes[i] << " ";
	o << ", and uri = " << _error_page.uri;

	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

AServerItem *OneLocation::addIndex(Node *node)
{
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
	if (this->_client_max_body_size == 1)
	{
		Node::t_inner_args_container values = node->get_inner_args();
		if (values.size() != 2)
			throw IncompleteDirective();
		_client_max_body_size = getIntNumberWithGuard(values[1]);
	}
	else
		throw MultipleDeclareError();
	return this;
}


AServerItem *OneLocation::addErrorPage(Node *node)
{
	if (1) // ! Todo: add condition for _error_code is added for the first time
	{
		Node::t_inner_args_container values = node->get_inner_args();
		if (values.size() < 3)
			throw IncompleteDirective();
		for (unsigned int i = 1; i < values.size() - 1; i++)
		{
			int code = getIntNumberWithGuard(values[i]);
			this->_error_page.errorCodes.push_back(code);
		}
		this->_error_page.uri = values[values.size() - 1];
	}
	else
		throw MultipleDeclareError();
	return this;
}

AServerItem *OneLocation::addCgi(Node *node)
{
	std::cout << "OneLocation I'm trying to add a cgi directive from " << *node;
	
	if (this->_cgi[0].compare("") == 0 && this->_cgi.size() == 1)
	{
		Node::t_inner_args_container values = node->get_inner_args();
		if (values.size() != 3)
			throw IncompleteDirective();
		_cgi.clear();
		for (unsigned long i = 1; i < values.size(); i++)
			_cgi.push_back(values[i]);
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
