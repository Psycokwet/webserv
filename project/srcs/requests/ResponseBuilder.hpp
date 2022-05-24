#ifndef RESPONSEBUILDER_HPP
#define RESPONSEBUILDER_HPP

#include <iostream>
#include <string>
#include <map>
#include "GrammarParser.hpp"

class GrammarParser;
class ResponseBuilder
{

public:
	ResponseBuilder(ResponseBuilder const &src);
	~ResponseBuilder();

	ResponseBuilder &operator=(ResponseBuilder const &rhs);
	static ResponseBuilder *build(std::map<std::string, std::string> &_parsed_datas, e_parsing_states endStatus);
	std::ostream &print(std::ostream &o) const;

private:
	ResponseBuilder(std::map<std::string, std::string> &_parsed_datas);
	std::map<std::string, std::string> _parsed_datas;
};

std::ostream &operator<<(std::ostream &o, ResponseBuilder const &i);

#endif /* ************************************************* RESPONSEBUILDER_H */