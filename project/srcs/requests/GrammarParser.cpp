#include "GrammarParser.hpp"

/*
** ---------------------------------- STATIC ----------------------------------
*/

t_builder_dictionary GrammarParser::initBuilderDictionnary()
{
    t_builder_dictionary vector;
	vector.push_back(t_pair_checker_consume(&check_OR, &consume_OR));
	vector.push_back(t_pair_checker_consume(&check_MULTI, &consume_MULTI));
	vector.push_back(t_pair_checker_consume(&check_INTERVAL, &consume_INTERVAL));
	vector.push_back(t_pair_checker_consume(&check_MULTIVALUES, &consume_MULTIVALUES));
	vector.push_back(t_pair_checker_consume(&check_STRING, &consume_STRING));
	vector.push_back(t_pair_checker_consume(&check_VAR, &consume_VAR));
    return map;
}

t_builder_dictionary GrammarParser::_directives_to_setter = GrammarParser::initBuilderDictionnary();
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

bool isGrammarMapValid(t_grammar_map &gramMap)
{
	(void)gramMap;
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
	if(isGrammarMapValid(vars))
		return new GrammarParser(vars);
	for(t_grammar_map::const_iterator it = vars.begin(); it != vars.end(); it++)
		delete it->second;
	return NULL;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarParser::GrammarParser(t_grammar_map vars) : _vars(vars)
{
}

GrammarParser::GrammarParser( const GrammarParser & src ) : _vars(src._vars)
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
	return o;
}

std::ostream &			operator<<( std::ostream & o, GrammarParser const & i )
{
	return i.print(o);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */