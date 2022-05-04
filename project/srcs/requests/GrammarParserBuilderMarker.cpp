#include "GrammarParserBuilderMarker.hpp"

DEFINE_ENUM(e_states, E_STATES_ENUM)
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarParserBuilderMarker::GrammarParserBuilderMarker(
	int deepness,
	GrammarVariables *gv)	:
		_deepness(deepness),
		_gv(gv),
		_tokenIndex(0),
		_min(1),
		_max(1),
		_count(0),
		_resetTo(0),
		_buffer("")
{
}

GrammarParserBuilderMarker::GrammarParserBuilderMarker(
	const GrammarParserBuilderMarker & src) :
		_deepness(src._deepness),
		_gv(src._gv),
		_tokenIndex(src._tokenIndex),
		_min(src._min),
		_max(src._max),
		_count(src._count),
		_resetTo(src._resetTo),
		_buffer(src._buffer)
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

void GrammarParserBuilderMarker::setRep(int min, int max)
{
	this->_min = min;
	this->_max = max;
}

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
	this->_count++;
	if(this->_count < this->_max)
	{
		this->_tokenIndex = this->_resetTo;
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


void GrammarParserBuilderMarker::addToBuffer(std::string buffer)
{
	this->_buffer += buffer;
}
std::string GrammarParserBuilderMarker::getBuffer()
{
	return this->_buffer;
}

GrammarVariables *GrammarParserBuilderMarker::getVar()
{
	return this->_gv;
}

/* ************************************************************************** */