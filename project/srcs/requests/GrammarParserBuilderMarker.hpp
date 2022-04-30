#ifndef GRAMMARPARSERBUILDERMARKER_HPP
# define GRAMMARPARSERBUILDERMARKER_HPP

# include <iostream>
# include <string>

class GrammarParserBuilderMarker
{

	public:

		GrammarParserBuilderMarker(int deepness = -1);
		GrammarParserBuilderMarker( GrammarParserBuilderMarker const & src );
		~GrammarParserBuilderMarker();

		GrammarParserBuilderMarker &		operator=( GrammarParserBuilderMarker const & rhs );
		std::ostream &			print( std::ostream & o) const;

	private:
		int _deepness;

};

std::ostream &			operator<<( std::ostream & o, GrammarParserBuilderMarker const & i );

#endif /* ************************************** GRAMMARPARSERBUILDERMARKER_H */