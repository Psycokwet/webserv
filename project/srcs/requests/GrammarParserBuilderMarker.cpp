#include "GrammarParserBuilderMarker.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarParserBuilderMarker::GrammarParserBuilderMarker(
	int resetRequestIndex,
	int deepness,
	GrammarVariables *gv,
	int tokenIndex,
	int resetLastId) : _deepness(deepness),
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
					   _resetLastId(resetLastId),
					   _resetRequestIndex(resetRequestIndex) //,
															 // _countCharRead(0)
{
	if (resetRequestIndex < 0)
		throw BuildError();
}

GrammarParserBuilderMarker::GrammarParserBuilderMarker(
	const GrammarParserBuilderMarker &src) : _deepness(src._deepness),
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
											 _resetLastId(src._resetLastId),
											 _resetRequestIndex(src._resetRequestIndex) //,
																						// _countCharRead(src._countCharRead)
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

GrammarParserBuilderMarker &GrammarParserBuilderMarker::operator=(GrammarParserBuilderMarker const &rhs)
{
	_deepness = rhs._deepness;
	_gv = rhs._gv;
	_tokenIndex = rhs._tokenIndex;
	// if ( this != &rhs )
	//{
	// _value = rhs.getValue();
	//}
	return *this;
}

std::ostream &operator<<(std::ostream &o, GrammarParserBuilderMarker const &i)
{
	return i.print(o);
}

std::ostream &GrammarParserBuilderMarker::print(std::ostream &o) const
{
	o << "WHOAMI [" << this << "] ; ";
	o << "requestIndex [" << getResetRequestIndex() << "] ; ";
	o << "_resetRequestIndex [" << _resetRequestIndex << "] ; ";
	o << "_lastId [" << _lastId << "] ; ";
	o << "_resetLastId [" << _resetLastId << "] ; ";
	o << "_confirmedBuffer [" << _confirmedBuffer << "] ; ";
	o << "_buffer [" << _buffer << "] ; ";
	o << "_deepness " << _deepness << " ; ";
	o << "_gv [" << *_gv << "] ; ";
	o << "_tokenIndex " << _tokenIndex << " ; ";
	o << "_count " << _count << " ; ";
	o << "_resetTo " << _resetTo << " ; ";
	o << "_maxIndexToken " << _maxIndexToken << " ; ";
	o << "_min " << _min << " ; ";
	o << "_max " << _max << " ; ";
	o << "_isCurrentlyValid " << (_isCurrentlyValid == true ? "true" : "false") << " ; ";
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

#define IS_OPENING_STATEMENT(x) (x == "(" || x == "[")
#define IS_CLOSING_STATEMENT(x) (x == ")" || x == "]")

int GrammarParserBuilderMarker::findMaxIndex() const
{
	if (!IS_OPENING_STATEMENT(getCurrentToken()))
		return _tokenIndex + 1;
	std::list<Statements> stats = std::list<Statements>();
	std::vector<std::string> &tokens = _gv->getTokens();
	int i = _tokenIndex;
	stats.push_back(Statements(_deepness, tokens[i], _gv));
	while (stats.size() != 0)
	{
		i++;
		if (i >= sizeTokens())
			return -1;
		if (IS_OPENING_STATEMENT(tokens[i]))
			stats.push_back(Statements(_deepness, tokens[i], _gv));
		else if (IS_CLOSING_STATEMENT(tokens[i]))
		{
			if (stats.size() == 0 || !stats.back().isTheRightClosingStatement(tokens[i], _gv))
				return -1;
			stats.pop_back();
		}
	}
	return i;
}

void GrammarParserBuilderMarker::setRep(int min, int max)
{
	_min = min;
	_max = max;
	_resetTo = _tokenIndex;
	_maxIndexToken = findMaxIndex();
	if (_maxIndexToken > (_tokenIndex + 1))
	{
		_tokenIndex++;
		_resetTo = _tokenIndex;
	}
}

std::string GrammarParserBuilderMarker::getCurrentToken() const
{
	if (_tokenIndex >= _maxIndexToken)
		throw TokenOutOfBound();
	return _gv->getTokens()[_tokenIndex];
}

void GrammarParserBuilderMarker::reset()
{
	_tokenIndex = _resetTo;
	_lastId = _resetLastId;
	_isCurrentlyValid = true;
	_confirmedBuffer += _buffer;
	_buffer = "";

	_count++;
}

bool GrammarParserBuilderMarker::canBeParsed()
{
	if (_maxIndexToken != _tokenIndex && !_isCurrentlyValid && getCurrentToken() != "/" && _lastId != INDEX_OR)
		return false;
	if (_maxIndexToken == _tokenIndex &&
		(_max <= (_count + 1) || !_isCurrentlyValid))
		return false;
	return true;
}

void GrammarParserBuilderMarker::prepareNextParsing()
{
	if (_maxIndexToken == _tokenIndex && _max > _count)
		reset();
}

bool GrammarParserBuilderMarker::incToken()
{
	if (_maxIndexToken > _tokenIndex)
	{
		_tokenIndex++;
		return true;
	}
	return false;
}

bool GrammarParserBuilderMarker::incTokenTo(int newIndex)
{
	if (_maxIndexToken >= newIndex)
	{
		_tokenIndex = newIndex;
		return true;
	}
	return false;
}

int GrammarParserBuilderMarker::sizeTokens() const
{
	return _gv->getTokens().size();
}

bool GrammarParserBuilderMarker::hasEnoughRep() const
{
	if (_count >= _min || (_count + 1 >= _min && _tokenIndex == _maxIndexToken && _isCurrentlyValid))
		return true;
	return false;
}
bool GrammarParserBuilderMarker::hasFinishedCurrentRep() const
{
	if (_tokenIndex >= _maxIndexToken)
		return true;
	return false;
}

void GrammarParserBuilderMarker::resetBuffer()
{
	_buffer = "";
}

bool GrammarParserBuilderMarker::isValidInTheEnd() const
{
	if ((getIsCurrentlyValid() && hasEnoughRep() && hasFinishedCurrentRep()) || hasEnoughRep())
		return true;
	return false;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
void GrammarParserBuilderMarker::setResetRequestIndex(int resetRequestIndex)
{
	_resetRequestIndex = resetRequestIndex;
}

int GrammarParserBuilderMarker::getResetRequestIndex() const
{
	if (_isCurrentlyValid)
		return _resetRequestIndex + _buffer.length() + _confirmedBuffer.length();
	return _resetRequestIndex + _confirmedBuffer.length();
}

int GrammarParserBuilderMarker::getResetTo() const
{
	return _resetTo;
}

int GrammarParserBuilderMarker::getDeepness() const
{
	return _deepness;
}

int GrammarParserBuilderMarker::getTokenIndex() const
{
	return _tokenIndex;
}

int GrammarParserBuilderMarker::getLastId() const
{
	return _lastId;
}

void GrammarParserBuilderMarker::setLastId(int id)
{
	_lastId = id;
}

bool GrammarParserBuilderMarker::getIsCurrentlyValid() const
{
	return _isCurrentlyValid;
}
void GrammarParserBuilderMarker::setIsCurrentlyValid(bool valid)
{
	_isCurrentlyValid = valid;
}

int GrammarParserBuilderMarker::getMaxIndexToken() const
{
	return _maxIndexToken;
}

void GrammarParserBuilderMarker::addToBuffer(std::string buffer)
{
	_buffer += buffer;
}

std::string GrammarParserBuilderMarker::getBuffer() const
{
	if (_isCurrentlyValid)
		return _confirmedBuffer + _buffer;
	return _confirmedBuffer;
}


std::string *GrammarParserBuilderMarker::getBufferPtr()
{
	return &_confirmedBuffer;
}

GrammarVariables *GrammarParserBuilderMarker::getVar()
{
	return _gv;
}

/* ************************************************************************** */