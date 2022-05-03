#ifndef GRAMMARPARSER_HPP
# define GRAMMARPARSER_HPP

# include "../util/parse.hpp"
# include <ctype.h>
# include <iostream>
# include <sstream>
# include <cstring>
# include <list>
# include <map>
# include <algorithm>
# include <vector>
# include "GrammarVariables.hpp"
# include "GrammarParserBuilderMarker.hpp"
# include "Statements.hpp"
# include <climits>



# include "../../includes/enumFactory.h"
# define E_PARSING_STATE_ENUM(XX) \
    XX(PARSE_SUCCESS,=0x00000001) \
    XX(PARSE_FAILURE,=0x00000010) \
    XX(PARSE_FATAL_FAILURE,=0x00000100) \
    XX(PARSE_NOT_ENOUGH_DATAS,=0x00001000) \
    XX(PARSE_NOTHING_MORE,=0x00010000) \
    XX(PARSE_QUIT_DEBUG,=0x00100000) \

DECLARE_ENUM(e_parsing_states, E_PARSING_STATE_ENUM)

typedef std::map<std::string, GrammarVariables*> t_grammar_map;
class GrammarParser;
					//checker	token			//vars					//consume				token			state request parsing
typedef std::pair<	bool (*)(	std::string, 	 t_grammar_map &gm),	e_parsing_states (GrammarParser::*)(	std::string,	GrammarParserBuilderMarker*)> t_pair_checker_consume;
typedef std::vector< t_pair_checker_consume > t_builder_dictionary;

#define ID_BASE_REQUEST "HTTP_message"
class GrammarParser
{
	public:

		GrammarParser( GrammarParser const & src );
		~GrammarParser();

		GrammarParser &		operator=( GrammarParser const & rhs );
		std::ostream &			print( std::ostream & o) const;

		static GrammarParser *build(std::string filename);
		static t_builder_dictionary _builderDictionnary;
		static t_builder_dictionary initBuilderDictionnary();

		e_parsing_states consume_OR(std::string token,	GrammarParserBuilderMarker *gp);
		e_parsing_states consume_MULTI(std::string token,	GrammarParserBuilderMarker *gp);
		e_parsing_states consume_VALUE(std::string token,	GrammarParserBuilderMarker *gp);
		e_parsing_states consume_QUOTEVALUE(std::string token,	GrammarParserBuilderMarker *gp);
		e_parsing_states consume_INTERVAL(std::string token,	GrammarParserBuilderMarker *gp);
		e_parsing_states consume_MULTIVALUES(std::string token,	GrammarParserBuilderMarker *gp);
		e_parsing_states consume_BLOCK(std::string token,	GrammarParserBuilderMarker *gp);
		e_parsing_states consume_STRING(std::string token,	GrammarParserBuilderMarker *gp);
		e_parsing_states consume_OPTIONAL(std::string token,	GrammarParserBuilderMarker *gp);
		e_parsing_states consume_VAR(std::string token,	GrammarParserBuilderMarker *gp);

		void feed(std::string buff);
		void initParse();
		e_parsing_states parse();
		void clear();
		class IllegalParsingState : public std::exception
        {
            public:
                virtual const char *what() const throw()
				{
					return "Parser encountered an error in its own process";
				}
        };

	private:
		GrammarParser(t_grammar_map vars = t_grammar_map(), std::string request = "");
		t_grammar_map _vars;
		std::size_t _requestIndex;
		std::string _request;
		std::list<GrammarParserBuilderMarker*> _priority_states;
		std::list<Statements*> _statementsOpened;
		
		std::string _key_buffer;
		std::string _value_buffer;
		std::string *_current_buffer;
		std::map<std::string, std::string> _parsed_datas;
		
		bool tryIncToken();
		bool addToCurrentBuffer(std::string add);

		

};

std::ostream &			operator<<( std::ostream & o, GrammarParser const & i );

#endif /* *************************************************** GRAMMARPARSER_H */