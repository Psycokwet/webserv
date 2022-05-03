#ifndef GRAMMARPARSERBUILDERMARKER_HPP
# define GRAMMARPARSERBUILDERMARKER_HPP

# include <iostream>
# include <string>
# include "GrammarVariables.hpp"
# include "../../includes/enumFactory.h"

# define E_STATES_ENUM(XX) \
    XX(NO_TYPE,=0x00000001) \
    XX(HASHMAP,=0x00000010) \
    XX(LIST,=0x00000100) \

DECLARE_ENUM(e_states, E_STATES_ENUM)

class GrammarParserBuilderMarker
{

	public:

		GrammarParserBuilderMarker(int deepness = -1, GrammarVariables *gv = NULL, int tokenIndex = -1);
		GrammarParserBuilderMarker( GrammarParserBuilderMarker const & src );
		~GrammarParserBuilderMarker();

		GrammarParserBuilderMarker &		operator=( GrammarParserBuilderMarker const & rhs );
		std::ostream &			print( std::ostream & o) const;

		GrammarVariables *getVar();
		std::string getCurrentToken();
		int getTokenIndex() const;
		bool incToken();
		int sizeTokens() const;
		int getDeepness() const;
		class TokenOutOfBound : public std::exception
        {
            public:
                virtual const char *what() const throw()
				{
					return "Trying to get an out of bound token";
				}
        };

	private:
		int _deepness;
		GrammarVariables *_gv;
		int _tokenIndex;

};

std::ostream &			operator<<( std::ostream & o, GrammarParserBuilderMarker const & i );

#endif /* ************************************** GRAMMARPARSERBUILDERMARKER_H */