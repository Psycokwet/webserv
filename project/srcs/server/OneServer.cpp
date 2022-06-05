#include "OneServer.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

DIRECTIVES_MAP OneServer::initializeDirectivesMap()
{
	DIRECTIVES_MAP map;
	map["location"] = &ALocation::addLocation;
	map["server_name"] = &ALocation::addServerName;
	map["listen"] = &ALocation::addListen;

	map["index"] = &ALocation::addIndex;
	map["root"] = &ALocation::addRoot;
	map["autoindex"] = &ALocation::addAutoIndex;
	map["method"] = &ALocation::addMethod;
	map["client_max_body_size"] = &ALocation::addMaxSize;
	map["error_page"] = &ALocation::addErrorPage;
	map["cgi"] = &ALocation::addCgi;
	return map;
}

DIRECTIVES_MAP OneServer::_directives_to_setter = OneServer::initializeDirectivesMap();

AServerItem *OneServer::consume(Node *node)
{
	if (!node)
		return NULL; // Or throw, whichever feels best

	Node::t_inner_args_container &inner_args = node->getInnerArgs(); // ! get all elements of server key (get all servers)
	if (inner_args.size() == 0)
		return NULL; // Or throw, whichever feels best

	// ! Get corresponding function to function pointer named directiveConsumer, based on the name of the directive
	AServerItem *(OneServer::*directiveConsumer)(Node *) = OneServer::_directives_to_setter[*(inner_args.begin())];

	if (!directiveConsumer)
		return NULL; // Or throw, whichever feels best
	// ! Call and Run the corresponding function, but how to change/add value to the same OneServer???
	// ! The function does some job and change inside value of OneServer object. (return this)
	return (this->*directiveConsumer)(node);
}

static std::vector<std::string> split(const std::string &s, char seperator)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}

uint32_t getDecimalValueOfIPV4_String(const char *ipAddress)
{
	uint8_t ipbytes[4] = {};
	unsigned int i = 0;
	int8_t j = 3;
	while (ipAddress + i && i < strlen(ipAddress))
	{
		char digit = ipAddress[i];
		if (isdigit(digit) == 0 && digit != '.')
			return 0;
		j = digit == '.' ? j - 1 : j;
		ipbytes[j] = ipbytes[j] * 10 + atoi(&digit);
		i++;
	}
	uint32_t a = ipbytes[0];
	uint32_t b = (uint32_t)ipbytes[1] << 8;
	uint32_t c = (uint32_t)ipbytes[2] << 16;
	uint32_t d = (uint32_t)ipbytes[3] << 24;
	return a + b + c + d;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

OneServer::OneServer() // Todo: put default value to each directive
{
	_server_name.push_back("");
	_listen = t_listen();
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

OneServer::~OneServer()
{
	for (std::map<std::string, OneLocation *>::const_iterator it = this->_location.begin(); it != this->_location.end(); it++)
		delete it->second;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::ostream &OneServer::print(std::ostream &o) const
{
	o << "\n\tI'm OneServer and I have as _server_name = ";
	for (unsigned long i = 0; i < _server_name.size(); i++)
		o << _server_name[i] << " ";

	o << "\n\t_listen: with address = " << _listen._address;
	o << ", port = " << _listen._port;
	o << ", default_server = " << _listen._default_server;

	o << "\n\t_index = ";
	for (unsigned long i = 0; i < _index.size(); i++)
		o << _index[i] << " ";
	o << "\n\t_root = " << _root;
	o << "\n\t_autoindex = " << _autoindex;
	o << "\n\t_method = ";
	for (std::set<std::string>::const_iterator it = _method.begin(); it != _method.end(); it++)
		o << *it << " ";
	o << "\n\t_client_max_body_size = " << _client_max_body_size;

	o << "\n\t_error_page: with codes = ";
	for (unsigned long i = 0; i < _error_page.errorCodes.size(); i++)
		o << _error_page.errorCodes[i] << " ";
	o << ", and uri = " << _error_page.uri;

	o << "\t_cgi = ";
	for (unsigned long i = 0; i < _cgi.size(); i++)
		o << _cgi[i] << " ";
	o << std::endl;
	for (std::map<std::string, OneLocation *>::const_iterator it = this->_location.begin(); it != this->_location.end(); it++)
	{
		o << "\n\t\t"
		  << "_location = " << it->first << "\t";
		(it->second)->print(o) << std::endl;
	}

	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/
AServerItem *OneServer::findRightConfig(std::list<std::string> route_target)
{

	if (route_target.size() == 0)
		return NULL;
	for (std::vector<std::string>::iterator server_name_it = _server_name.begin(); server_name_it != _server_name.end(); server_name_it++)
	{
		if (route_target.front() != *server_name_it)
			continue;
		std::list<std::string>::iterator route_target_it = route_target.begin();
		route_target_it++;
		if (route_target.size() > 1)
		{
			for (std::map<std::string, OneLocation *>::iterator it = _location.begin(); it != _location.end(); it++)
			{
				if (*route_target_it != it->first)
					continue;
				return it->second;
			}
		}
		return this;
	}
	return NULL;
}

AServerItem *OneServer::addServerName(Node *node)
{
	if (this->_server_name[0].compare("") == 0 && this->_server_name.size() == 1)
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
	Node::t_inner_args_container values = node->get_inner_args();
	if (values.size() != 2) // necessary because juste after you try to read values[1]
		throw IncompleteDirective();
	OneLocation *location_value = new OneLocation(this);
	if (_location.find(values[1]) != _location.end())
		throw DuplicateUriError();
	_location[values[1]] = location_value;
	return location_value;
}

AServerItem *OneServer::addListen(Node *node)
{
	std::cout << "OneServer I'm trying to add a listen directive from " << *node;

	if (_listen._port == DEFAULT_PORT && _listen._address == LOCALHOST && _listen._default_server.compare("") == 0)
	{
		Node::t_inner_args_container values = node->get_inner_args();
		if (values.size() < 2 || values.size() > 3)
			throw InvalidValueError();
		if (values.size() == 3)
		{
			_listen._default_server.assign(values[2]);
			std::vector<std::string> output = split(values[1], ':');
			if (output.size() == 2)
			{
				_listen._address = getDecimalValueOfIPV4_String(output[0].c_str());
				_listen._port = getIntNumberWithGuard(output[1]);
			}
			else if (output.size() == 1)
			{
				if (isIntNumber(output[0]))
					_listen._port = getIntNumberWithGuard(output[0]);
				else
					_listen._address = getDecimalValueOfIPV4_String(output[0].c_str());
			}
		}
		else if (values.size() == 2)
		{
			if (isIntNumber(values[1]))
				_listen._port = getIntNumberWithGuard(values[1]);
			else
				_listen._address = getDecimalValueOfIPV4_String(values[1].c_str());
		}
	}
	else
		throw MultipleDeclareError();
	return this;
}

AServerItem *OneServer::addIndex(Node *node)
{
	if (this->_index[0].compare("index.html") == 0 && this->_index.size() == 1)
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
	if (this->_method.begin()->compare("GET") == 0 && this->_method.size() == 1)
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

AServerItem *OneServer::addErrorPage(Node *node)
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

AServerItem *OneServer::addCgi(Node *node)
{
	std::cout << "OneServer I'm trying to add a cgi directive from " << *node;

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

DIRECTIVES_MAP &OneServer::getDirectiveMap()
{
	return this->_directives_to_setter;
}

t_listen OneServer::getListen()
{
	return _listen;
}

/* ************************************************************************** */