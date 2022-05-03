#include "GrammarParserBuilderMarker.hpp"

DEFINE_ENUM(e_states, E_STATES_ENUM)
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarParserBuilderMarker::GrammarParserBuilderMarker(int deepness, GrammarVariables *gv, int tokenIndex) :_deepness(deepness), _gv(gv), _tokenIndex(tokenIndex)
{
}

GrammarParserBuilderMarker::GrammarParserBuilderMarker( const GrammarParserBuilderMarker & src ) :_deepness(src._deepness), _gv(src._gv), _tokenIndex(src._tokenIndex)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

GrammarParserBuilderMarker::~GrammarParserBuilderMarker()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

GrammarParserBuilderMarker &				GrammarParserBuilderMarker::operator=( GrammarParserBuilderMarker const & rhs )
{
	this->_deepness = rhs._deepness;
	this->_gv = rhs._gv;
	this->_tokenIndex = rhs._tokenIndex;
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, GrammarParserBuilderMarker const & i )
{
	return i.print(o);
}


std::ostream &			GrammarParserBuilderMarker::print( std::ostream & o) const
{
	o << "this->_deepness " << this->_deepness << " ; ";
	o << "this->_gv [" << *this->_gv << "] ; ";
	o << "this->_tokenIndex " << this->_tokenIndex << " ; ";
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


std::string GrammarParserBuilderMarker::getCurrentToken()
{
	if(this->_tokenIndex >= sizeTokens())
		throw new TokenOutOfBound();
	return this->_gv->getTokens()[this->_tokenIndex];
}

bool GrammarParserBuilderMarker::incToken()
{
	if(sizeTokens() > this->_tokenIndex + 1)
	{
		this->_tokenIndex++;
		return true;
	}
	return false;
}
int GrammarParserBuilderMarker::sizeTokens() const
{
	return this->_gv->getTokens().size();
}

int GrammarParserBuilderMarker::getDeepness() const
{
	return this->_deepness;
}

int GrammarParserBuilderMarker::getTokenIndex() const
{
	return this->_tokenIndex;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

GrammarVariables *GrammarParserBuilderMarker::getVar()
{
	return this->_gv;
}

/* ************************************************************************** */