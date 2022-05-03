#include "GrammarParser.hpp"

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
	tokens.erase(tokens.begin(), tokens.begin() + 2);
	GrammarVariables *gv = new GrammarVariables(name, tokens);
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

bool checkValidityForVar(GrammarVariables *gv, t_grammar_map &gramMap)
{
	std::vector<std::string> &tokens = gv->getTokens();
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if(getBuilderIDForToken(tokens[i], gramMap) == NON_VALID)
			return false;
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
	if(isGrammarMapValid(vars) || vars.find(ID_BASE_REQUEST) != vars.end())
		return new GrammarParser(vars);
	for(t_grammar_map::const_iterator it = vars.begin(); it != vars.end(); it++)
		delete it->second;
	return NULL;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarParser::GrammarParser(t_grammar_map vars, std::string request) : _vars(vars), _request(request), _priority_states(), _statementsOpened()
{
}

GrammarParser::GrammarParser( const GrammarParser & src ) : _vars(src._vars), _request(src._request), _priority_states(src._priority_states), _statementsOpened(src._statementsOpened)
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

bool GrammarParser::tryIncToken(GrammarParserBuilderMarker *gp)
{
	if(!gp->incToken())
	{
	 	delete gp;
		_priority_states.pop_front();
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
	this->_request = "";
}

void GrammarParser::initParse()
{
	if (_priority_states.size() == 0)
		_priority_states.push_back(new GrammarParserBuilderMarker(0, this->_vars[ID_BASE_REQUEST], 0));
}
int GrammarParser::parse()
{
	initParse();
	std::string token = _priority_states.front()->getCurrentToken();
	int id = getBuilderIDForToken(token, this->_vars);
	if(id == NON_VALID)
		return -1;
	(this->*GrammarParser::_builderDictionnary[id].second)(token, _priority_states.front());
	return 0;
	// std::vector<std::string> &tokens = gv->getTokens();
	// for (size_t i = 0; i < tokens.size(); i++)
	// {
	// 	if(getBuilderIDForToken(tokens[i], gramMap) == NON_VALID)
	// 		return false;
	// }
	// return true;
}


int GrammarParser::consume_OR(std::string token,	GrammarParserBuilderMarker *gp)
{
	(void)token;
	(void)gp;
	return 0;
}

int GrammarParser::consume_MULTI(std::string token,	GrammarParserBuilderMarker *gp)
{
	(void)token;
	(void)gp;
	return 0;
}

int GrammarParser::consume_VALUE(std::string token,	GrammarParserBuilderMarker *gp)
{
	(void)token;
	(void)gp;
	return 0;
}

int GrammarParser::consume_QUOTEVALUE(std::string token,	GrammarParserBuilderMarker *gp)
{
	(void)token;
	(void)gp;
	return 0;
}

int GrammarParser::consume_INTERVAL(std::string token,	GrammarParserBuilderMarker *gp)
{
	(void)token;
	(void)gp;
	return 0;
}

int GrammarParser::consume_MULTIVALUES(std::string token,	GrammarParserBuilderMarker *gp)
{
	(void)token;
	(void)gp;
	return 0;
}

int GrammarParser::consume_BLOCK(std::string token,	GrammarParserBuilderMarker *gp)
{
	(void)token;
	(void)gp;
	return 0;
}

int GrammarParser::consume_STRING(std::string token,	GrammarParserBuilderMarker *gp)
{
	(void)token;
	(void)gp;
	return 0;
}

int GrammarParser::consume_OPTIONAL(std::string token,	GrammarParserBuilderMarker *gp)
{
	if (token == "[")
	{
		this->_statementsOpened.push_back(new Statements(gp->getDeepness(), "[", gp->getVar()));
 		tryIncToken(gp);
		
	}
	else
	{
		if(this->_statementsOpened.size() == 0 || !this->_statementsOpened.back()->isTheRightClosingStatement("]", gp->getVar()))
			throw new IllegalParsingState();
		tryIncToken(gp);
	}
	(void)token;
	(void)gp;
	return 0;
}


int GrammarParser::consume_VAR(std::string token,	GrammarParserBuilderMarker *gp)
{
 	tryIncToken(gp);
	_priority_states.push_front(new GrammarParserBuilderMarker(gp->getDeepness() + 1, this->_vars[token], 0));
	return 0;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */