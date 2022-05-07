#ifndef GRAMMARPARSERBUILDERMARKER_HPP
# define GRAMMARPARSERBUILDERMARKER_HPP

# include <iostream>
# include <string>
# include <list>
# include "GrammarVariables.hpp"
# include "Statements.hpp"
# include "../../includes/enumFactory.h"
# include "GrammarParser.hpp"

# define E_STATES_ENUM(XX) \
    XX(NO_TYPE,=0x00000001) \
    XX(HASHMAP,=0x00000010) \
    XX(LIST,=0x00000100) \

DECLARE_ENUM(e_states, E_STATES_ENUM)

class GrammarParserBuilderMarker
{

	public:

		GrammarParserBuilderMarker(int deepness = -1, GrammarVariables *gv = NULL, int tokenIndex = 0);
		GrammarParserBuilderMarker( GrammarParserBuilderMarker const & src );
		~GrammarParserBuilderMarker();

		GrammarParserBuilderMarker &		operator=( GrammarParserBuilderMarker const & rhs );
		std::ostream &			print( std::ostream & o) const;

		GrammarVariables *getVar();
		std::string getCurrentToken() const;
		int getTokenIndex() const;
		bool incToken();
		int sizeTokens() const;
		int getDeepness() const;
		void setRep(int min, int max);


		class TokenOutOfBound : public std::exception
        {
            public:
                virtual const char *what() const throw()
				{
					return "Trying to get an out of bound token";
				}
        };
		void addToBuffer(std::string buffer);
		std::string getBuffer();
		bool incTokenTo(int newIndex);
		int getMaxIndexToken() const;
		bool getIsCurrentlyValid() const;
		void setIsCurrentlyValid(bool valid);
		int findMaxIndex() const;
		int getLastId() const;
		void setLastId(int id);
		bool hasEnoughRep() const;
		bool hasFinishedCurrentRep() const;
		

	private:

		int _deepness;
		GrammarVariables *_gv;
		int _tokenIndex;
		int _min;
		int _max;
		int _count;
		int _resetTo;
		std::string _buffer;
		int _maxIndexToken;
		int _isCurrentlyValid;
		int _lastId;
		

};

std::ostream &			operator<<( std::ostream & o, GrammarParserBuilderMarker const & i );

#endif /* ************************************** GRAMMARPARSERBUILDERMARKER_H */