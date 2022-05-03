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


typedef std::map<std::string, GrammarVariables*> t_grammar_map;
class GrammarParser;
					//checker	token			//vars					//consume				token			state request parsing
typedef std::pair<	bool (*)(	std::string, 	 t_grammar_map &gm),	int (GrammarParser::*)(	std::string,	GrammarParserBuilderMarker*)> t_pair_checker_consume;
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

		int consume_OR(std::string token,	GrammarParserBuilderMarker *gp);
		int consume_MULTI(std::string token,	GrammarParserBuilderMarker *gp);
		int consume_VALUE(std::string token,	GrammarParserBuilderMarker *gp);
		int consume_QUOTEVALUE(std::string token,	GrammarParserBuilderMarker *gp);
		int consume_INTERVAL(std::string token,	GrammarParserBuilderMarker *gp);
		int consume_MULTIVALUES(std::string token,	GrammarParserBuilderMarker *gp);
		int consume_BLOCK(std::string token,	GrammarParserBuilderMarker *gp);
		int consume_STRING(std::string token,	GrammarParserBuilderMarker *gp);
		int consume_OPTIONAL(std::string token,	GrammarParserBuilderMarker *gp);
		int consume_VAR(std::string token,	GrammarParserBuilderMarker *gp);

		void feed(std::string buff);
		void initParse();
		int parse();
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
		std::string _requestIndex;
		std::string _request;
		std::list<GrammarParserBuilderMarker*> _priority_states;
		std::list<Statements*> _statementsOpened;
		
		bool  tryIncToken(GrammarParserBuilderMarker *gp);
		

};

std::ostream &			operator<<( std::ostream & o, GrammarParser const & i );

#endif /* *************************************************** GRAMMARPARSER_H */