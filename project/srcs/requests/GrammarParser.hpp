#ifndef GRAMMARPARSER_HPP
#define GRAMMARPARSER_HPP

#define INDEX_OR 0
#define INDEX_MULTI 1
#define INDEX_VALUE 2
#define INDEX_QUOTEVALUE 3
#define INDEX_INTERVAL 4
#define INDEX_MULTIVALUES 5
#define INDEX_BLOCK 6
#define INDEX_STRING 7
#define INDEX_OPTIONAL 8
#define INDEX_VAR 9

#define RESET "\033[0m"
#define BLACK "\033[30m"			  /* Black */
#define RED "\033[31m"				  /* Red */
#define GREEN "\033[32m"			  /* Green */
#define YELLOW "\033[33m"			  /* Yellow */
#define BLUE "\033[34m"				  /* Blue */
#define MAGENTA "\033[35m"			  /* Magenta */
#define CYAN "\033[36m"				  /* Cyan */
#define WHITE "\033[37m"			  /* White */
#define BOLDBLACK "\033[1m\033[30m"	  /* Bold Black */
#define BOLDRED "\033[1m\033[31m"	  /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"	  /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"	  /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"	  /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"	  /* Bold White */

#include "../../includes/enumFactory.h"
#define E_PARSING_STATE_ENUM(XX)          \
	XX(PARSE_SUCCESS, = 1)                \
	XX(PARSE_NOT_ENOUGH_DATAS, = 2)       \
	XX(PARSE_NOTHING_MORE, = 3)           \
	XX(PARSE_FAILURE, = 4)                \
	XX(PARSE_FATAL_FAILURE, = 5)          \
	XX(PARSE_UNEXPECTED_END_PATTERN, = 6) \
	XX(PARSE_QUIT_DEBUG, = 7)

DECLARE_ENUM(e_parsing_states, E_PARSING_STATE_ENUM)

#include "../util/parse.hpp"
#include <ctype.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <list>
#include <map>
#include <algorithm>
#include <vector>
#include "GrammarVariables.hpp"
#include "GrammarParserBuilderMarker.hpp"
#include "ResponseBuilder.hpp"
#include <climits>

typedef std::map<std::string, GrammarVariables *> t_grammar_map;
class GrammarParser;
class ResponseBuilder;
class GrammarParserBuilderMarker;
// checker	token			//vars					//consume							token		state request parsing		id command
typedef std::pair<bool (*)(std::string, t_grammar_map &gm), e_parsing_states (GrammarParser::*)(std::string, GrammarParserBuilderMarker *, int)> t_pair_checker_consume;
typedef std::vector<t_pair_checker_consume> t_builder_dictionary;

#define ID_BASE_REQUEST "HTTP_message"
class GrammarParser
{
public:
	GrammarParser(GrammarParser const &src);
	~GrammarParser();

	GrammarParser &operator=(GrammarParser const &rhs);
	std::ostream &print(std::ostream &o) const;

	static GrammarParser *build(std::string filename);
	static t_builder_dictionary _builderDictionnary;
	static t_builder_dictionary initBuilderDictionnary();

	e_parsing_states consume_OR(std::string token, GrammarParserBuilderMarker *gp, int id);
	e_parsing_states consume_MULTI(std::string token, GrammarParserBuilderMarker *gp, int id);
	e_parsing_states consume_VALUE(std::string token, GrammarParserBuilderMarker *gp, int id);
	e_parsing_states consume_QUOTEVALUE(std::string token, GrammarParserBuilderMarker *gp, int id);
	e_parsing_states consume_INTERVAL(std::string token, GrammarParserBuilderMarker *gp, int id);
	e_parsing_states consume_MULTIVALUES(std::string token, GrammarParserBuilderMarker *gp, int id);
	e_parsing_states consume_BLOCK(std::string token, GrammarParserBuilderMarker *gp, int id);
	e_parsing_states consume_STRING(std::string token, GrammarParserBuilderMarker *gp, int id);
	e_parsing_states consume_OPTIONAL(std::string token, GrammarParserBuilderMarker *gp, int id);
	e_parsing_states consume_VAR(std::string token, GrammarParserBuilderMarker *gp, int id);

	e_parsing_states consume_STATEMENTS(std::string token, GrammarParserBuilderMarker *gp, int id, std::string statement, int min);

	void feed(std::string buff);
	void initParse();
	e_parsing_states parse();
	ResponseBuilder *finishParse();
	void clear();
	class IllegalParsingState : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return "Parser encountered an error in its own process";
		}
	};
	ResponseBuilder *getResponseBuilder();

private:
	GrammarParser(t_grammar_map vars = t_grammar_map(), std::string request = "");
	t_grammar_map _vars;
	std::string _request;
	std::list<GrammarParserBuilderMarker *> _priority_states;

	std::map<std::string, std::string> _parsed_datas;
	e_var_type _saveType;
	ResponseBuilder *_resp;

	void deleteFrontPriority();
	bool saveIfNecesary();
	bool tryIncToken();
	e_parsing_states resolveValidityOfOpenedLoops();
};

std::ostream &operator<<(std::ostream &o, GrammarParser const &i);

#endif /* *************************************************** GRAMMARPARSER_H */