#include "ResponseBuilder.hpp"

/*
** ------------------------------- STATIC --------------------------------
*/

ResponseBuilder *ResponseBuilder::build(std::map<std::string, std::string> &_parsed_datas, e_parsing_states endStatus)
{
	ResponseBuilder *resp = new ResponseBuilder(_parsed_datas);
	(void)endStatus;
	return resp;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ResponseBuilder::ResponseBuilder(std::map<std::string, std::string> &_parsed_datas) : _parsed_datas(std::map<std::string, std::string>(_parsed_datas))
{
}

ResponseBuilder::ResponseBuilder(const ResponseBuilder &src)
{
	(void)src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ResponseBuilder::~ResponseBuilder()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ResponseBuilder &ResponseBuilder::operator=(ResponseBuilder const &rhs)
{
	// if ( this != &rhs )
	//{
	// this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

std::ostream &ResponseBuilder::print(std::ostream &o) const
{
	o << "Base request :" << std::endl;
	return print_cont(o, _parsed_datas, "]\n", "[", ": ");
}

std::ostream &operator<<(std::ostream &o, ResponseBuilder const &i)
{
	return i.print(o);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */