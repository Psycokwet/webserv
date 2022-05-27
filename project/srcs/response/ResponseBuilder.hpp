#ifndef RESPONSEBUILDER_HPP
#define RESPONSEBUILDER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "../requests/GrammarParser.hpp"
# include "../util/deleteContainers.hpp"
# include "../util/print.hpp"

class GrammarParser;
class ResponseBuilder
{

public:
	ResponseBuilder(ResponseBuilder const &src);
	~ResponseBuilder();

	ResponseBuilder &operator=(ResponseBuilder const &rhs);
	static ResponseBuilder *init();
	ResponseBuilder *end_build(std::string raw, e_parsing_states endStatus);
	std::ostream &print(std::ostream &o) const;
	std::ostream &print_request(std::ostream &o) const;
	std::ostream &print_response(std::ostream &o) const;

	static const std::map<int, std::string> initReturnCodes();
	static const std::map<int, std::string> http_return_codes;
	void add_value_parsedDatas(std::string value);
	void add_key_parsedDatas(std::string key);
	std::string get_key_parsedDatas() const;
	std::string *get_value_buffer_parsedDatas() const;
	void set_value_buffer_parsedDatas(std::string *value_buffer);

	class KeyNotDeclared : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return "No key stored at the moment";
		}
	};class KeyAlreadyDeclared : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return "This key is erasing the previous stored";
		}
	};
	class KeyAlreadyUsed : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return "This value will erase the value stored previously at this key";
		}
	};
	class ValueBufferAlreadyDeclared : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return "A value buffer have been previously set and not consumed";
		}
	};
	void set_parsing_validity_state(e_parsing_states parsing_validity_state);
	e_parsing_states get_parsing_validity_state() const;
private:
	ResponseBuilder();
	std::map<std::string, std::string> _parsed_datas;
	std::vector<std::string> _insert_order_resp_cat;
	std::map<std::string, std::string> _resp_cat;
	std::string _key;
	std::string *_value_buffer;
	e_parsing_states _parsing_validity_state;
};

std::ostream &operator<<(std::ostream &o, ResponseBuilder const &i);

#endif /* ************************************************* RESPONSEBUILDER_H */