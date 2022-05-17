#include "GrammarParser.hpp"

DEFINE_ENUM(e_parsing_states, E_PARSING_STATE_ENUM)
/*
** ---------------------------------- LOCAL ----------------------------------
*/

bool isItTwoDigitHexa(std::string token)
{
	if(token.length() != 2 || ! isxdigit(token.at(0)) || ! isxdigit(token.at(1)))
		return false;
	return true;
}

bool check_OR (std::string token, t_grammar_map &gm)
{
	(void)gm;
	if(token == "/")
		return true;
	return false;
}

bool check_MULTI (std::string token, t_grammar_map &gm)
{
	(void)gm;
	size_t multiPos = token.find("*");
	if (multiPos != std::string::npos && token.find("*", multiPos + 1) != std::string::npos)
		return false;
	std::string tmp_block;
	std::stringstream ss(token);
	while(std::getline(ss, tmp_block, '*'))
	{
		for (size_t i = 0; i < tmp_block.length(); i++)
		{
			if(! isdigit(tmp_block.at(i)))
				return false;
		}
	}
	return true;
}

bool check_VALUE (std::string token, t_grammar_map &gm)
{
	(void)gm;
	if (token.length() == 4 && token.rfind("0x", 0) == 0 && isItTwoDigitHexa(token.substr(2, 4)))
		return true;
	return false;
}

bool check_QUOTEVALUE (std::string token, t_grammar_map &gm)
{
	(void)gm;
	if (token.length() == 3 && token.at(0) == '\'' && token.at(2) == '\'')
		return true;
	return false;
}

bool check_INTERVAL (std::string token, t_grammar_map &gm)
{
	(void)gm;
	std::string tmp_block;
	if (token.length() > 4 && (token.length() - 4) % 3 == 0 && token.rfind("0x", 0) == 0)
	{
		std::stringstream ss(token.substr(2, token.length()));
		while(std::getline(ss, tmp_block, '-'))
		{
			if (!isItTwoDigitHexa(tmp_block))
				return false;
		}
		return true;
	}
	return false;
}

bool check_MULTIVALUES (std::string token, t_grammar_map &gm)
{
	(void)gm;
	std::string tmp_block;
	if (token.length() > 4 && (token.length() - 4) % 3 == 0 && token.rfind("0x", 0) == 0)
	{
		std::stringstream ss(token.substr(2, token.length()));
		while(std::getline(ss, tmp_block, '.'))
		{
			if (!isItTwoDigitHexa(tmp_block))
				return false;
		}
		return true;
	}
	return false;
}

bool check_BLOCK (std::string token, t_grammar_map &gm)
{
	(void)gm;
	if(token == "(" || token == ")")
		return true;
	return false;
}

bool check_STRING (std::string token, t_grammar_map &gm)
{
	(void)gm;
	if (token.length() >= 2 && token.at(0) == '"' && token.at(token.length() - 1) == '"' )
		return true;
	return false;
}

bool check_OPTIONAL (std::string token, t_grammar_map &gm)
{
	(void)gm;
	if(token == "[" || token == "]")
		return true;
	return false;
}

bool check_VAR (std::string token, t_grammar_map &gm)
{
	if (gm.find(token) != gm.end())
		return true;
	return false;
}

/*
** ---------------------------------- STATIC ----------------------------------
*/

t_builder_dictionary GrammarParser::initBuilderDictionnary()
{
    t_builder_dictionary vector;
	vector.push_back(std::make_pair(&check_OR, &GrammarParser::consume_OR)); // /
	vector.push_back(std::make_pair(&check_MULTI, &GrammarParser::consume_MULTI)); // x*y
	vector.push_back(std::make_pair(&check_VALUE, &GrammarParser::consume_VALUE)); //  0x35
	vector.push_back(std::make_pair(&check_QUOTEVALUE, &GrammarParser::consume_QUOTEVALUE)); //  'x'
	vector.push_back(std::make_pair(&check_INTERVAL, &GrammarParser::consume_INTERVAL)); //  0x35-45
	vector.push_back(std::make_pair(&check_MULTIVALUES, &GrammarParser::consume_MULTIVALUES)); // 0x45.35
	vector.push_back(std::make_pair(&check_BLOCK, &GrammarParser::consume_BLOCK)); // ()
	vector.push_back(std::make_pair(&check_STRING, &GrammarParser::consume_STRING)); // ""
	vector.push_back(std::make_pair(&check_OPTIONAL, &GrammarParser::consume_OPTIONAL)); // []
	vector.push_back(std::make_pair(&check_VAR, &GrammarParser::consume_VAR)); //known var

	return vector;
}

t_builder_dictionary GrammarParser::_builderDictionnary = GrammarParser::initBuilderDictionnary();

#define PREPARE_AND_SKIP_EMPTY_LIGNES(str) std::replace_if(str.begin(), str.end(), isblank, ' '); \
		trim(str);\
		if(str == "" || str[0] == ';')\
			continue;

GrammarVariables *parseVar(std::string tmp_line)
{
	std::string tmp_block;
	std::string name;
	std::string type = GetString(NO_VAR_TYPE);
	std::vector<std::string> tokens;
	std::stringstream ss(tmp_line);
	while(std::getline(ss, tmp_block, ' '))
	{
		if(tmp_block[0] == ';')
			break;
		if(tmp_block == "")
			continue;
		tokens.push_back(tmp_block);
	}
	if(tokens.size() <= 2 || tokens[1] != "=")
		return NULL;
	name = tokens.front();
	if(name.find("!") != std::string::npos)
	{
		std::stringstream ss(name);
		if(!std::getline(ss, tmp_block, '!'))
			return NULL;
		type = tmp_block;
		if(!std::getline(ss, tmp_block, '!'))
			return NULL;
		name = tmp_block;
	}
	tokens.erase(tokens.begin(), tokens.begin() + 2);
	GrammarVariables *gv = new GrammarVariables(name, tokens, Gete_var_typeValue(type.c_str()));
	return gv;
}

#define NON_VALID -1

int getBuilderIDForToken(std::string token, t_grammar_map &gramMap)
{
	for (size_t i = 0; i < GrammarParser::_builderDictionnary.size(); i++)
	{
		if(GrammarParser::_builderDictionnary[i].first(token, gramMap))
			return i;
	}
	return NON_VALID;
}

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

static const char *COLORS[] = {
  BOLDGREEN,
  YELLOW,
  RED,
  MAGENTA,
  BOLDRED,
  WHITE,
  BOLDBLUE,
  BOLDMAGENTA,
  BOLDCYAN,
  BOLDWHITE,
  BOLDYELLOW,
  BLUE,
  CYAN,
  GREEN,
  BOLDBLACK,
  BLACK,
  NULL
};


bool checkValidityForVar(GrammarVariables *gv, t_grammar_map &gramMap)
{
	std::vector<std::string> &tokens = gv->getTokens();
	int id = NON_VALID;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if((id = getBuilderIDForToken(tokens[i], gramMap)) == NON_VALID)
			return false;
		if (!DEBUG)
			std::cout << COLORS[id] << " "<<tokens[i] << " "<< RESET;
	}
	return true;
}

bool isGrammarMapValid(t_grammar_map &gramMap)
{
	for(t_grammar_map::const_iterator it = gramMap.begin(); it != gramMap.end(); it++)
	{
		if (!checkValidityForVar((*it).second, gramMap))
			return false;
	}
	return true;
}

GrammarParser *GrammarParser::build(std::string filename)
{
	std::ifstream	ifs(filename.c_str());
	std::string		tmp_line = "";
	GrammarVariables *gv = NULL;
	t_grammar_map vars;

	if (!ifs.is_open())
	{
		std::cout << "Grammar innaccessible or non existing, can not proceed with request parsing" << std::endl;
		ifs.close();
		return NULL;
	}
	while(std::getline(ifs, tmp_line))
	{
		PREPARE_AND_SKIP_EMPTY_LIGNES(tmp_line)
		if((gv = parseVar(tmp_line)) == NULL)
		{
			ifs.close();
			std::cout << "The grammar file is not correctly formated" << std::endl;
			for(t_grammar_map::const_iterator it = vars.begin(); it != vars.end(); it++)
				delete it->second;
			return NULL;
		}
		vars[gv->getName()] = gv;
	}
	ifs.close();
	if(isGrammarMapValid(vars)&& vars.find(ID_BASE_REQUEST) != vars.end())
		return new GrammarParser(vars);
	for(t_grammar_map::const_iterator it = vars.begin(); it != vars.end(); it++)
		delete it->second;
	return NULL;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarParser::GrammarParser(t_grammar_map vars, std::string request) :
	_vars(vars),
	_request(request),
	_priority_states(),
	_statementsOpened(),
	_key_buffer(""),
	_value_buffer(""),
	_current_buffer(NULL),
	_deepnessMinBeforeSave(-1),
	_saveType(NO_VAR_TYPE),
	_indexTokenInitSave(-1)
{
}

GrammarParser::GrammarParser( const GrammarParser & src ) :
	_vars(src._vars),
	_request(src._request),
	_priority_states(src._priority_states),
	_statementsOpened(src._statementsOpened),
	_key_buffer(src._key_buffer),
	_value_buffer(src._value_buffer),
	_current_buffer(src._current_buffer),
	_deepnessMinBeforeSave(src._deepnessMinBeforeSave),
	_saveType(src._saveType),
	_indexTokenInitSave(src._indexTokenInitSave)
{
	(void)src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

GrammarParser::~GrammarParser()
{
	for(t_grammar_map::const_iterator it = this->_vars.begin(); it != this->_vars.end(); it++)
		delete it->second;
	for(std::list<GrammarParserBuilderMarker*>::const_iterator it = this->_priority_states.begin(); it != this->_priority_states.end(); it++)
		delete *it;
	for(std::list<Statements*>::const_iterator it = this->_statementsOpened.begin(); it != this->_statementsOpened.end(); it++)
		delete *it;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

GrammarParser &				GrammarParser::operator=( GrammarParser const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

std::ostream & GrammarParser::print(std::ostream & o) const
{
	for(t_grammar_map::const_iterator it = this->_vars.begin(); it != this->_vars.end(); it++)
		o << *(it->second) << std::endl;
	o << "Priorities :" << this->_priority_states.size() << std::endl;
	for(std::list<GrammarParserBuilderMarker*>::const_iterator it = this->_priority_states.begin(); it != this->_priority_states.end(); it++)
		o << "\t" << **it << std::endl;
	return o;
}

std::ostream &			operator<<( std::ostream & o, GrammarParser const & i )
{
	return i.print(o);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void GrammarParser::addToBuffer(std::string add, GrammarParserBuilderMarker *gp)
{
	this->_requestIndex += add.size();
	
	if (_saveType != NO_VAR_TYPE)
	{
		gp->addToBuffer(add);
		if (!DEBUG)
			std::cout << add << " [" << this->_request.substr(this->_requestIndex, 10).at(0) << ":" << this->_request.substr(this->_requestIndex, 10).at(1) << "]\n";
	}
}


bool GrammarParser::saveIfNecesary()
{
	e_var_type frontType = _priority_states.front()->getVar()->getType();
	std::cout << "saveIfNecesary "<< *_priority_states.front() << ":::"<<GetString(_saveType) <<":::"<<_indexTokenInitSave <<":::"<<_deepnessMinBeforeSave<<":::"<< _priority_states.front()->getDeepness() << std::endl;
	if (frontType == ONLY_VALUE || frontType == VALUE)
	{
		std::cout<<"VALIDATE CONTENT BUFFER :"<<this->_key_buffer<<":::"<< _priority_states.front()->getBuffer()<<std::endl;
		if(this->_parsed_datas.find(this->_key_buffer) != this->_parsed_datas.end())
		{		
			std::cout<< RESET <<"THROW CONTENT BUFFER :"<<this->_key_buffer<<":::"<< this->_parsed_datas[this->_key_buffer] <<"::" << _priority_states.front()->getBuffer()<<std::endl;
			throw new IllegalParsingState();
		}
		this->_parsed_datas[this->_key_buffer] = _priority_states.front()->getBuffer();
		this->_key_buffer = "";
		this->_current_buffer = NULL;
		_priority_states.front()->resetBuffer();
		_saveType = NO_VAR_TYPE;
		return true;
	}
	else if (frontType == KEY)
	{
		this->_key_buffer = _priority_states.front()->getBuffer();
		_priority_states.front()->resetBuffer();
		_saveType = NO_VAR_TYPE;
		return true;
	}
	return false;
}

void GrammarParser::deleteFrontPriority()
{	
	// std::cout<<"*SHOIUDL deleting:"<< (!_priority_states.front()->getIsCurrentlyValid()== true ? "true": "false" ) << ":::"<< (!_priority_states.front()->hasFinishedCurrentRep()== true ? "true": "false" )<< ":::"<<(willAddNewFrontAfter== true ? "true": "false" )<<std::endl;
	// if(_priority_states.front()->getIsCurrentlyValid() && !_priority_states.front()->hasFinishedCurrentRep())
	// 	return;
	// if(!_priority_states.front()->getIsCurrentlyValid() && ( !_priority_states.front()->hasFinishedCurrentRep() || willAddNewFrontAfter) && _priority_states.front()->getLastId() == INDEX_OR)
	// 	return;
	if (!_priority_states.front()->getIsCurrentlyValid() && _priority_states.front()->hasEnoughRep())
		_priority_states.front()->setIsCurrentlyValid(true);

	std::cout<<"deleting:"<< *_priority_states.front() << "\nRESULT : "<<( _priority_states.front()->isValidInTheEnd() == true ? "true": "false" ) <<std::endl;
	if (_priority_states.front()->isValidInTheEnd())
	{
		std::cout <<"SAVE BUFFER ??? " << GetString(_saveType) <<":::"<<_priority_states.size()<< std::endl;
		if(_priority_states.size() >= 2)
		{
			std::list<GrammarParserBuilderMarker *>::iterator it = _priority_states.begin();
			it++;
			(*it)->setIsCurrentlyValid(true);
			if(!saveIfNecesary() && _saveType != NO_VAR_TYPE){
				std::cout << "FRONT IS : " << *_priority_states.front() <<std::endl;
				std::cout << "SECOND IS : " << (**it) <<std::endl;
				(*it)->addToBuffer(_priority_states.front()->getBuffer());}
		}else 
			saveIfNecesary();
	}
	else if(_priority_states.size() > 2)
	{
		// bool toDeleteValidity = _priority_states.front()->getIsCurrentlyValid();
		delete _priority_states.front();
		_priority_states.pop_front();
		if(!_priority_states.front()->getIsCurrentlyValid() && _priority_states.front()->hasEnoughRep() && _priority_states.front()->hasFinishedCurrentRep())
			return deleteFrontPriority();
		return;
	}
	delete _priority_states.front();
	_priority_states.pop_front();
}

bool GrammarParser::tryIncToken()
{
	// std::cout<<"try inc \n";
	// 	return;
	if(!_priority_states.front()->incToken() && !_priority_states.front()->canBeParsed()
		// && !(
		// 	!_priority_states.front()->getIsCurrentlyValid()
		// 	&& (
		// 		(
		// 			!_priority_states.front()->hasFinishedCurrentRep()
		// 			&& _priority_states.front()->getTokenIndex() != _priority_states.front()->getResetTo()
		// 		)
		// 		|| (
		// 			willAddNewFrontAfter
		// 			&& _priority_states.front()->getTokenIndex() != _priority_states.front()->getResetTo()
		// 		)
		// 	)
		// 	&& _priority_states.front()->getLastId() == INDEX_OR
		// )
	)

	{
		deleteFrontPriority();
		return false;
	}
	return true;
}


void GrammarParser::feed(std::string buff)
{
	this->_request += buff;
}

void GrammarParser::clear()
{
	this->_parsed_datas.clear();
	this->_request = "";
	this->_requestIndex = 0;
	this->_value_buffer = "";
	this->_key_buffer = "";
	this->_current_buffer = NULL;
}

void GrammarParser::initParse()
{
	if (_priority_states.size() != 0)
		return;
	this->_parsed_datas.clear();
	this->_value_buffer = "";
	this->_key_buffer = "";
	this->_current_buffer = NULL;
	this->_requestIndex = 0;
	_priority_states.push_back(new GrammarParserBuilderMarker(0, this->_vars[ID_BASE_REQUEST]));
}

e_parsing_states GrammarParser::parse()
{
	initParse();

int i = 0;
	e_parsing_states current;
	do {
		if(this->_requestIndex >= this->_request.size())
			return PARSE_NOTHING_MORE;
		if(i++ > 15){

	for (std::map<std::string, std::string>::iterator it= this->_parsed_datas.begin(); it !=this->_parsed_datas.end() ; it++)
	{
		std::cout << "RESULT PARSING : [" << it->first << ":::"<< it->second << "]" <<std::endl;
		/* code */
	}
			return PARSE_QUIT_DEBUG;}
		while(_priority_states.size() > 1 &&!_priority_states.front()->canBeParsed())
		{
			// std::cout<< "CANT BE PA4RSED " <<*_priority_states.front() <<std::endl;

			resolveValidityOfOpenedLoops();
		}
		if(_priority_states.size() == 0)
			return PARSE_UNEXPECTED_END_PATTERN;
		_priority_states.front()->prepareNextParsing();
		std::string token = _priority_states.front()->getCurrentToken();
		int id = getBuilderIDForToken(token, this->_vars);
		if(id == NON_VALID)
			return PARSE_FATAL_FAILURE;
		if (!DEBUG)
			std::cout << COLORS[id];
		std::cout<< *_priority_states.front() <<"::"<< id <<"::"<< this->_requestIndex << "::"<<std::endl;
		GrammarParserBuilderMarker *gp = _priority_states.front();
		try
		{
			// std::cout << token <<" for ["<< this->_request.substr(this->_requestIndex, 3)<<"]"<< std::endl;
			current = (this->*GrammarParser::_builderDictionnary[id].second)(token, gp, id);
			// std::cout << GetString(current) << std::endl;
		}
		catch(const std::exception& e)
		{
			if (!DEBUG)
				std::cout << RESET;
			std::cerr << e.what() << " from " << id  << '\n';
			return current;
		}

		if (!DEBUG)
			std::cout << RESET;
		if (current == PARSE_FAILURE)
		{		

			// std::cout<< "PARSE FAILURE !!!"<<std::endl;
			gp->setIsCurrentlyValid(false);
			// std::cout<< *_priority_states.front()<<std::endl;
			// if( _priority_states.front()->hasFinishedCurrentRep())
			// 	deleteFrontPriority();
		}
	}
	while(current != PARSE_NOT_ENOUGH_DATAS && current != PARSE_FATAL_FAILURE);

	for (std::map<std::string, std::string>::iterator it= this->_parsed_datas.begin(); it !=this->_parsed_datas.end() ; it++)
	{
		std::cout << "RESULT PARSING : " << it->first << ":::"<< it->second << std::endl;
		/* code */
	}

	return current;
	// std::vector<std::string> &tokens = gv->getTokens();
	// for (size_t i = 0; i < tokens.size(); i++)
	// {
	// 	if(getBuilderIDForToken(tokens[i], gramMap) == NON_VALID)
	// 		return false;
	// }
	// return true;
}


e_parsing_states GrammarParser::consume_OR(std::string token,	GrammarParserBuilderMarker *gp, int id)
{
	_priority_states.front()->setLastId(id);
	// std::cout<<"STATE " << (gp->getIsCurrentlyValid() == true ? "true": "false") << "::::" << *_priority_states.front() <<"\n";

	if(gp->getIsCurrentlyValid())
	{
		if(!tryIncToken())
			return PARSE_FATAL_FAILURE;
		int i = gp->findMaxIndex();
		if(i == -1)
			return PARSE_FATAL_FAILURE;
		std::cout << i << " is what we have here\n";
		gp->incTokenTo(i);
		// tryIncToken();
		// std::cout << "SHOULD HAVE PASSED NEXT PATTERN " << *gp<<std::endl;
		return PARSE_SUCCESS;
	}
	if(!tryIncToken())
		return PARSE_FATAL_FAILURE;
	(void)token;
	return PARSE_SUCCESS;
}

e_parsing_states GrammarParser::consume_MULTI(std::string token,	GrammarParserBuilderMarker *gp, int id)
{
	// if(! gp->getIsCurrentlyValid() && gp->getLastId() != INDEX_OR)
	// 	return resolveValidityOfOpenedLoops(true);
	// if(!gp->getIsCurrentlyValid())
	// 	return PARSE_FATAL_FAILURE;
	if(!tryIncToken())
		return PARSE_FATAL_FAILURE;
	std::string tmp_block;
	int min = 0;
	int max = INT_MAX;
	std::stringstream ss(token);
	if(!std::getline(ss, tmp_block, '*'))
		return PARSE_FATAL_FAILURE;
	if(tmp_block.size() > 0)
		min = strtol(tmp_block.c_str(), NULL, 10);

	if(std::getline(ss, tmp_block, '*') && tmp_block.size() > 0)
		max = strtol(tmp_block.c_str(), NULL, 10);
	GrammarParserBuilderMarker* loopgp = new GrammarParserBuilderMarker(gp->getDeepness() + 1, gp->getVar(), gp->getTokenIndex(), id);
	loopgp->setRep(min, max);
	int check = loopgp->getMaxIndexToken();
	if (check == -1)
		return PARSE_FATAL_FAILURE;
	gp->incTokenTo(check);
	tryIncToken();
	if(gp == _priority_states.front())
		_priority_states.front()->setLastId(id);
	_priority_states.push_front(loopgp);
	return PARSE_SUCCESS;
}

e_parsing_states GrammarParser::consume_VALUE(std::string token,	GrammarParserBuilderMarker *gp, int id)
{
	// if(! gp->getIsCurrentlyValid() && gp->getLastId() != INDEX_OR)
	// 	return resolveValidityOfOpenedLoops();
	// if(! gp->getIsCurrentlyValid() && gp->getLastId() != INDEX_OR)
	// 	return PARSE_FATAL_FAILURE;
	long valid_value = strtol(token.substr(2).c_str(), NULL, 16);
	char c = this->_request.at(this->_requestIndex);
	std::string tmp ;
	tmp += c;
	if(c != valid_value)
	{
		tryIncToken();
		if(gp == _priority_states.front())
			_priority_states.front()->setLastId(id);
		return PARSE_FAILURE;
	}
	// std::cout<<"CHAR IS :" << tmp << " NEEED "<< valid_value<<std::endl;
	addToBuffer(tmp, gp);
	gp->setIsCurrentlyValid(true);
	tryIncToken();
	if(gp == _priority_states.front())
		_priority_states.front()->setLastId(id);
	return PARSE_SUCCESS;
}

e_parsing_states GrammarParser::consume_QUOTEVALUE(std::string token,	GrammarParserBuilderMarker *gp, int id)
{
	return consume_STRING(token, gp, id);
}

e_parsing_states GrammarParser::consume_INTERVAL(std::string token,	GrammarParserBuilderMarker *gp, int id)
{
	// if(! gp->getIsCurrentlyValid() && gp->getLastId() != INDEX_OR)
	// 	return resolveValidityOfOpenedLoops();
	// if(! gp->getIsCurrentlyValid() && gp->getLastId() != INDEX_OR)
	// 	return PARSE_FATAL_FAILURE;
	long valid_min = strtol(token.substr(2, 4).c_str(), NULL, 16);
	long valid_max = strtol(token.substr(5, 7).c_str(), NULL, 16);
	char c = this->_request.at(this->_requestIndex);
	if(c > valid_max || c < valid_min)
	{
		tryIncToken();
		if(gp == _priority_states.front())
			_priority_states.front()->setLastId(id);
		return PARSE_FAILURE;
	}
	std::string tmp ;
	tmp += c;
	// std::cout<< "CHAR IS :" << tmp << " NEEED "<< valid_min<<":"<< valid_max<<std::endl;
	addToBuffer(tmp, gp);
	gp->setIsCurrentlyValid(true);
	tryIncToken();
	if(gp == _priority_states.front())
		_priority_states.front()->setLastId(id);
	return PARSE_SUCCESS;
}

e_parsing_states GrammarParser::consume_MULTIVALUES(std::string token,	GrammarParserBuilderMarker *gp, int id)
{
	throw IllegalParsingState();
	// if(! gp->getIsCurrentlyValid() && gp->getLastId() != INDEX_OR)
	// 	return resolveValidityOfOpenedLoops();
	_priority_states.front()->setLastId(id);
	tryIncToken();
	if(gp == _priority_states.front())
		_priority_states.front()->setLastId(id);
	(void)token;
	(void)gp;
	return PARSE_SUCCESS;
}


e_parsing_states GrammarParser::consume_STATEMENTS(std::string token,	GrammarParserBuilderMarker *gp, int id, std::string statement, int min)
{
	// if(! gp->getIsCurrentlyValid() && gp->getLastId() != INDEX_OR)
	// 	return resolveValidityOfOpenedLoops(true);
	if (token == statement)
	{
		// std::cout <<" TRying to generate a statemen 1::"<< *_priority_states.front()<< ":::" << id <<std::endl;
		// std::cout <<" TRying to generate a statemen 2::"<< gp->getMaxIndexToken() << ":::" << gp->getTokenIndex() + 1<<std::endl;
		if(gp->getMaxIndexToken() <= gp->getTokenIndex() + 1)
			return PARSE_FATAL_FAILURE;
		// std::cout <<" TRying to generate a statemen " <<gp->getDeepness() + 1 <<":::"<< *gp->getVar()<<":::"<< gp->getTokenIndex() + 1<< std::endl;

		GrammarParserBuilderMarker* loopgp = new GrammarParserBuilderMarker(gp->getDeepness() + 1, gp->getVar(), gp->getTokenIndex(), id);
		loopgp->setRep(min, 1);
		int check = loopgp->getMaxIndexToken();
		if (check == -1)
			return PARSE_FATAL_FAILURE;
		gp->incTokenTo(check);
		tryIncToken();
		if(gp == _priority_states.front())
			_priority_states.front()->setLastId(id);
		_priority_states.push_front(loopgp);
	}
	else
	{
		tryIncToken();
		if(gp == _priority_states.front())
			_priority_states.front()->setLastId(id);
	}
	return PARSE_SUCCESS;
}

e_parsing_states GrammarParser::consume_BLOCK(std::string token,	GrammarParserBuilderMarker *gp, int id)
{
	return consume_STATEMENTS(token, gp, id, "(", 1);
}

e_parsing_states GrammarParser::resolveValidityOfOpenedLoops()
{
	// std::cout << " RESOLVE LOOPS" << _priority_states.front()->getIsCurrentlyValid() <<":::"<< _priority_states.front()->getLastId() <<":::"<<  INDEX_OR<<":::"<<_priority_states.front()->hasEnoughRep()<<":::"<< std::endl;
	deleteFrontPriority();
	if(
		//( !_priority_states.front()->getIsCurrentlyValid() && _priority_states.front()->getLastId() != INDEX_OR) &&
		( !_priority_states.front()->getIsCurrentlyValid() && _priority_states.front()->hasEnoughRep()))
	{
		_priority_states.front()->setIsCurrentlyValid(true);
		return resolveValidityOfOpenedLoops();
	}
	if(_priority_states.front()->getIsCurrentlyValid())
		return PARSE_SUCCESS;
	return PARSE_FAILURE;
	// } else if (_priority_states.front()->getIsCurrentlyValid()
	// 	&& _priority_states.front()->hasEnoughRep()
	// 	&& _priority_states.front()->hasFinishedCurrentRep()) {

	//  }
	// return PARSE_FAILURE;
	// return PARSE_FATAL_FAILURE;
	// return PARSE_SUCCESS;
}

e_parsing_states GrammarParser::consume_STRING(std::string token,	GrammarParserBuilderMarker *gp, int id)
{
	// std::cout <<"CONSUME STRING "<< (gp->getIsCurrentlyValid() == true ? "true": "false")<<":::"<<gp->getLastId()<<":::"<<INDEX_OR<<std::endl;
	// if(! gp->getIsCurrentlyValid() && gp->getLastId() != INDEX_OR)
	// 	return resolveValidityOfOpenedLoops();
	std::string substring;
	std::string subToken = token.substr(1, token.size() - 2);
	if (this->_request.size() - this->_requestIndex < subToken.size())
		return PARSE_NOT_ENOUGH_DATAS;
	substring = this->_request.substr(this->_requestIndex, subToken.size());
	// std::cout << "comparing : " << substring << ":::" << subToken<<std::endl;
	if (substring != subToken)
	{
		tryIncToken();
		if(gp == _priority_states.front())
			_priority_states.front()->setLastId(id);
		return PARSE_FAILURE;
	}
	addToBuffer(substring, gp);
	gp->setIsCurrentlyValid(true);
	tryIncToken();
	if(gp == _priority_states.front())
		_priority_states.front()->setLastId(id);
	return PARSE_SUCCESS;
}

e_parsing_states GrammarParser::consume_OPTIONAL(std::string token,	GrammarParserBuilderMarker *gp, int id)
{
	return consume_STATEMENTS(token, gp, id, "[", 0);
}


e_parsing_states GrammarParser::consume_VAR(std::string token,	GrammarParserBuilderMarker *gp, int id)
{
	// std::cout << " should ? LOOPS valid:" <<( _priority_states.front()->getIsCurrentlyValid() == true ? "true":"false" )<<"::id:"<< _priority_states.front()->getLastId() <<"::id_or:"<<  INDEX_OR<<"::has enough:"<<(_priority_states.front()->hasEnoughRep()== true ? "true":"false")<<":::"<< std::endl;
	// if(! gp->getIsCurrentlyValid() && gp->getLastId() != INDEX_OR)
	// 	return resolveValidityOfOpenedLoops(true);
	// if(! gp->getIsCurrentlyValid() && gp->getLastId() != INDEX_OR)
	// {
	// 	gp->incTokenTo(gp->getMaxIndexToken());
	// 	gp->setIsCurrentlyValid(false);
 	// 	tryIncToken();
	// 	return PARSE_FAILURE;
	// }
	int tmp_deepness = gp->getDeepness();
	GrammarVariables *gv =  this->_vars[token];
	if (gv->getType() == ONLY_VALUE)
	{
		// _deepnessMinBeforeSave = tmp_deepness;
		_saveType = gv->getType();
		// _indexTokenInitSave = gp->getTokenIndex() + 1;
		this->_key_buffer = "#" + gv->getName();
		this->_current_buffer = &this->_value_buffer;
	}
	else if (gv->getType() == KEY)
	{
		// _deepnessMinBeforeSave = tmp_deepness;
		_saveType = gv->getType();
		// _indexTokenInitSave = gp->getTokenIndex() + 1;
		this->_current_buffer = &this->_key_buffer;
	}
	else if (gv->getType() == VALUE)
	{
		// _deepnessMinBeforeSave = tmp_deepness;
		_saveType = gv->getType();
		// _indexTokenInitSave = gp->getTokenIndex() + 1;
		this->_current_buffer = &this->_value_buffer;
	}
 	tryIncToken();
	if(gp == _priority_states.front())
		_priority_states.front()->setLastId(id);
	_priority_states.push_front(new GrammarParserBuilderMarker(tmp_deepness + 1, gv));
	return PARSE_SUCCESS;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */