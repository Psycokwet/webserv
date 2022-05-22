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

class GrammarParser;
class GrammarParserBuilderMarker
{

	public:

		GrammarParserBuilderMarker(int resetRequestIndex = -1, int deepness = -1, GrammarVariables *gv = NULL, int tokenIndex = 0, int resetLastId = 11);
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
		class BuildError : public std::exception
        {
            public:
                virtual const char *what() const throw()
				{
					return "Trying to set invalid values at build time";
				}
        };

		void addToBuffer(std::string buffer);
		std::string getBuffer();
		bool incTokenTo(int newIndex);
		int getMaxIndexToken() const;
		int getResetTo() const;
		bool getIsCurrentlyValid() const;
		void setIsCurrentlyValid(bool valid);
		int findMaxIndex() const;
		int getLastId() const;
		void setLastId(int id);
		bool hasEnoughRep() const;
		bool hasFinishedCurrentRep() const;
		void resetBuffer();
		bool canBeParsed();
		void prepareNextParsing();
		void reset();
		bool isValidInTheEnd() const;
		int getResetRequestIndex() const;
		void setResetRequestIndex(int resetRequestIndex);

	private:

		int _deepness;
		GrammarVariables *_gv;
		int _tokenIndex;
		int _min;
		int _max;
		int _count;
		int _resetTo;
		std::string _buffer;
		std::string _confirmedBuffer;
		int _maxIndexToken;
		int _isCurrentlyValid;
		int _lastId;
		int _resetLastId;
		int _resetRequestIndex;
		// int _countCharRead;
		

};

std::ostream &			operator<<( std::ostream & o, GrammarParserBuilderMarker const & i );

#endif /* ************************************** GRAMMARPARSERBUILDERMARKER_H */