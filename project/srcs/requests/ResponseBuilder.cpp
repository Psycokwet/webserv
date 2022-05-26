#include "ResponseBuilder.hpp"

/*
** ------------------------------- STATIC --------------------------------
*/
const std::map<int, std::string> ResponseBuilder::initReturnCodes()
{
	std::map<int, std::string> errorCodes;
	errorCodes[100] = "Continue";
	errorCodes[101] = "Switching Protocols";

	errorCodes[200] = "OK";
	errorCodes[201] = "Created";
	errorCodes[202] = "Accepted";
	errorCodes[203] = "Non-Authoritative Information";
	errorCodes[204] = "No Content";
	errorCodes[205] = "Reset Content";
	errorCodes[206] = "Partial Content";

	errorCodes[300] = "Multiple Choices";
	errorCodes[301] = "Moved Permanently";
	errorCodes[302] = "Found";
	errorCodes[303] = "See Other";
	errorCodes[304] = "Not Modified";
	errorCodes[305] = "Use Proxy";
	errorCodes[306] = "Temporary Redirect";

	errorCodes[400] = "Bad Request";
	errorCodes[401] = "Unauthorized";
	errorCodes[402] = "Payment Required";
	errorCodes[403] = "Forbidden";
	errorCodes[404] = "Not Found";
	errorCodes[405] = "Method Not Allowed";
	errorCodes[406] = "Not Acceptable";
	errorCodes[407] = "Proxy Authentication Required";
	errorCodes[408] = "Request Time-out";
	errorCodes[409] = "Conflict";
	errorCodes[410] = "Gone";
	errorCodes[411] = "Length Required";
	errorCodes[412] = "Precondition Failed";
	errorCodes[413] = "Request Entity Too Large";
	errorCodes[414] = "Request-URI Too Large";
	errorCodes[415] = "Unsupported Media Type";
	errorCodes[416] = "Requested range not satisfiable";
	errorCodes[417] = "Expectation Failed";
	errorCodes[418] = "I'm a teapot";

	errorCodes[500] = "Internal Server Error";
	errorCodes[500] = "Not Implemented";
	errorCodes[500] = "Bad Gateway";
	errorCodes[500] = "Service Unavailable";
	errorCodes[500] = "Gateway Time-out";
	errorCodes[500] = "HTTP Version not supported";



	return errorCodes;
}

const std::map<int, std::string> ResponseBuilder::http_return_codes = ResponseBuilder::initReturnCodes();

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