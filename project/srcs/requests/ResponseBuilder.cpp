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

ResponseBuilder *ResponseBuilder::init()
{
	ResponseBuilder *resp = new ResponseBuilder();
	return resp;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ResponseBuilder::ResponseBuilder() :
	_parsed_datas(std::map<std::string, std::string>()),
	_insert_order_resp_cat(std::vector<std::string>()), 
	_resp_cat(std::map<std::string, std::string>()),
	_key(""),
	_value_buffer(NULL),
	_parsing_validity_state(PARSE_NOT_ENOUGH_DATAS)
{
}

ResponseBuilder::ResponseBuilder(const ResponseBuilder &src):
	_parsed_datas(src._parsed_datas),
	_insert_order_resp_cat(std::vector<std::string>(src._insert_order_resp_cat)), 
	_resp_cat(std::map<std::string, std::string>(src._resp_cat)),
	_key(src._key),
	_value_buffer(src._value_buffer),
	_parsing_validity_state(src._parsing_validity_state)
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
	(void)rhs;
	return *this;
}

std::ostream &ResponseBuilder::print_request(std::ostream &o) const
{
	return print_cont(o, _parsed_datas, "]\n", "[", ": ");
}

std::ostream &ResponseBuilder::print_response(std::ostream &o) const
{
	return print_cont(o, _parsed_datas, "]\n", "[", ": ");
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

ResponseBuilder *ResponseBuilder::end_build(std::string raw, e_parsing_states endStatus)
{
	(void)raw;
	(void)endStatus;
	return this;
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


void ResponseBuilder::add_value_parsedDatas(std::string value)
{
	if (this->_parsed_datas.find(this->_key) != this->_parsed_datas.end())
		throw KeyAlreadyUsed();
	if(_key == "")
		throw KeyNotDeclared();
	this->_parsed_datas[_key] = value;
	this->_key = "";
	this->_value_buffer = NULL;
}

void ResponseBuilder::add_key_parsedDatas(std::string key)
{
	if(this->_value_buffer != NULL)
		throw ValueBufferAlreadyDeclared();
	if(_key != "")
		throw KeyAlreadyDeclared();
	this->_key = key;
}

std::string ResponseBuilder::get_key_parsedDatas() const
{
	if(_key == "")
		throw KeyNotDeclared();
	return this->_key;
}

std::string *ResponseBuilder::get_value_buffer_parsedDatas() const
{
	return _value_buffer;
}
void ResponseBuilder::set_value_buffer_parsedDatas(std::string *value_buffer)
{
	if(this->_value_buffer != NULL)
		throw ValueBufferAlreadyDeclared();
	this->_value_buffer = value_buffer;
}

void ResponseBuilder::set_parsing_validity_state(e_parsing_states parsing_validity_state)
{
	this->_parsing_validity_state = parsing_validity_state;
}

e_parsing_states ResponseBuilder::get_parsing_validity_state() const
{
	return this->_parsing_validity_state;
}

/* ************************************************************************** */