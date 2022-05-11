#include "GrammarParserBuilderMarker.hpp"

DEFINE_ENUM(e_states, E_STATES_ENUM)
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarParserBuilderMarker::GrammarParserBuilderMarker(
	int deepness,
	GrammarVariables *gv,
	int tokenIndex,
	int resetLastId )	:
		_deepness(deepness),
		_gv(gv),
		_tokenIndex(tokenIndex),
		_min(1),
		_max(1),
		_count(0),
		_resetTo(0),
		_buffer(""),
		_confirmedBuffer(""),
		_maxIndexToken(sizeTokens()),
		_isCurrentlyValid(true),
		_lastId(GrammarParser::_builderDictionnary.size() + 1),
		_resetLastId(resetLastId)
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
		_buffer(src._buffer),
		_confirmedBuffer(src._confirmedBuffer),
		_maxIndexToken(src._maxIndexToken),
		_isCurrentlyValid(src._isCurrentlyValid),
		_lastId(src._lastId),
		_resetLastId(src._resetLastId)
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
	o << "this->_lastId [" << this->_lastId << "] ; ";
	o << "this->_resetLastId [" << this->_resetLastId << "] ; ";
	o << "this->_confirmedBuffer [" << this->_confirmedBuffer << "] ; ";
	o << "this->_buffer [" << this->_buffer << "] ; ";
	o << "this->_deepness " << this->_deepness << " ; ";
	o << "this->_gv [" << *this->_gv << "] ; ";
	o << "this->_tokenIndex " << this->_tokenIndex << " ; ";
	o << "this->_count " << this->_count << " ; ";
	o << "this->_resetTo " << this->_resetTo << " ; ";
	o << "this->_maxIndexToken " << this->_maxIndexToken << " ; ";
	o << "this->_min " << this->_min << " ; ";
	o << "this->_max " << this->_max << " ; ";
	o << "this->_isCurrentlyValid " << (this->_isCurrentlyValid == true ? "true": "false") << " ; ";
	o << "this->_gv [" << *this->_gv << "] ; ";
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

#define IS_OPENING_STATEMENT(x) (x == "(" || x == "[")
#define IS_CLOSING_STATEMENT(x) (x == ")" || x == "]")

int GrammarParserBuilderMarker::findMaxIndex() const
{
	// std::cout << "STARTING WITH " << *this << ":::"<< this->getCurrentToken()<<std::endl;
	if(!IS_OPENING_STATEMENT(this->getCurrentToken()))
		return this->_tokenIndex;
	// std::cout << "STILL ON BABY" <<std::endl;
	std::list<Statements> stats = std::list<Statements> ();
	std::vector<std::string> &tokens = this->_gv->getTokens();
	int i = this->_tokenIndex;
	stats.push_back(Statements(this->_deepness, tokens[i], this->_gv));
	while (stats.size() != 0)
	{
		i++;
	// std::cout << "STILL ON BABY :" <<i<<std::endl;
		if(i >= sizeTokens())
			return -1;
		if (IS_OPENING_STATEMENT(tokens[i]))
			stats.push_back(Statements(this->_deepness, tokens[i], this->_gv));
		else if (IS_CLOSING_STATEMENT(tokens[i]))
		{
			if(stats.size() == 0 || !stats.back().isTheRightClosingStatement(tokens[i], this->_gv))
				return -1;
			stats.pop_back();
		}
	}
	return i;
}

void GrammarParserBuilderMarker::setRep(int min, int max)
{
	this->_min = min;
	this->_max = max;
	this->_resetTo = this->_tokenIndex;
	this->_maxIndexToken = findMaxIndex();
	if(_maxIndexToken > _tokenIndex)
	{
		this->_tokenIndex++;
		this->_resetTo = this->_tokenIndex;
	}
	// std::cout << "SET REP FOR " << *this << std::endl;
}

std::string GrammarParserBuilderMarker::getCurrentToken() const
{
	if(this->_tokenIndex >= sizeTokens())
		throw new TokenOutOfBound();
	return this->_gv->getTokens()[this->_tokenIndex];
}

bool GrammarParserBuilderMarker::incToken()
{
	if(this->_maxIndexToken > this->_tokenIndex + 1)
	{
		this->_tokenIndex++;
		return true;
	}
	this->_count++;
	if(this->_count < this->_max && _isCurrentlyValid)
	{
		this->_tokenIndex = this->_resetTo;
		this->_lastId = this->_resetLastId;
		this->_isCurrentlyValid = true;
		this->_confirmedBuffer += _buffer;
		this->_buffer = "";
		return true;
	}
	return false;
}

bool GrammarParserBuilderMarker::incTokenTo(int newIndex)
{
	if(this->_maxIndexToken > newIndex)
	{
		this->_tokenIndex = newIndex;
		return true;
	}
	return false;
}

int GrammarParserBuilderMarker::sizeTokens() const
{
	return this->_gv->getTokens().size();
}

bool GrammarParserBuilderMarker::hasEnoughRep() const
{
	if(this->_count >= this->_min)
		return true;
	return false;
}
bool GrammarParserBuilderMarker::hasFinishedCurrentRep() const
{
	if(this->_tokenIndex < (this->_maxIndexToken -1))
		return false;
	return true;
}

void GrammarParserBuilderMarker::resetBuffer()
{
	this->_buffer = "";
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/
int GrammarParserBuilderMarker::getResetTo() const
{
	return _resetTo;
}


int GrammarParserBuilderMarker::getDeepness() const
{
	return this->_deepness;
}

int GrammarParserBuilderMarker::getTokenIndex() const
{
	return this->_tokenIndex;
}

int GrammarParserBuilderMarker::getLastId() const
{
	return this->_lastId;
}

void GrammarParserBuilderMarker::setLastId(int id)
{
	this->_lastId = id;
}

bool GrammarParserBuilderMarker::getIsCurrentlyValid() const
{
	return this->_isCurrentlyValid;
}
void GrammarParserBuilderMarker::setIsCurrentlyValid(bool valid)
{
	this->_isCurrentlyValid = valid;
}

int GrammarParserBuilderMarker::getMaxIndexToken() const
{
	return this->_maxIndexToken;
}

void GrammarParserBuilderMarker::addToBuffer(std::string buffer)
{
	this->_buffer += buffer;
}
std::string GrammarParserBuilderMarker::getBuffer()
{
	if(!_isCurrentlyValid)
	std::cout <<*this<<  "\n trying to retrieve " << this->_confirmedBuffer <<":::"<< this->_buffer<< " with "<<_isCurrentlyValid <<std::endl;
	if(_isCurrentlyValid)
		return this->_confirmedBuffer + this->_buffer;
	return this->_confirmedBuffer;
}

GrammarVariables *GrammarParserBuilderMarker::getVar()
{
	return this->_gv;
}

/* ************************************************************************** */