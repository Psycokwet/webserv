#ifndef GRAMMARPARSER_HPP
# define GRAMMARPARSER_HPP

# include "../util/parse.hpp"
# include <ctype.h>
# include <iostream>
# include <sstream>
# include <cstring>
# include <list>
# include <map>
# include <vector>
# include "GrammarVariables.hpp"
# include "GrammarParserBuilderMarker.hpp"


typedef std::map<std::string, GrammarVariables*> t_grammar_map;
class GrammarParser
{						//checker				token			//consume										token			state request parsing
	typedef std::pair<bool (GrammarParser::*)(	std::string),	GrammarParserBuilderMarker (GrammarParser::*)(	std::string,	GrammarParserBuilderMarker)> t_pair_checker_consume;
	typedef std::vector< t_pair_checker_consume > t_builder_dictionary;
	public:

		GrammarParser( GrammarParser const & src );
		~GrammarParser();

		GrammarParser &		operator=( GrammarParser const & rhs );
		std::ostream &			print( std::ostream & o) const;

		static GrammarParser *build(std::string filename);
		static t_builder_dictionary _builderDictionnary;
		static t_builder_dictionary initBuilderDictionnary();

	private:
		GrammarParser(t_grammar_map vars = t_grammar_map());
		t_grammar_map _vars;

};

std::ostream &			operator<<( std::ostream & o, GrammarParser const & i );

#endif /* *************************************************** GRAMMARPARSER_H */