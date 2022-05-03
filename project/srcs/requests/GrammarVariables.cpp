#include "GrammarVariables.hpp"
DEFINE_ENUM(e_var_type, E_VAR_TYPE_ENUM)

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarVariables::GrammarVariables(std::string name, std::vector<std::string> tokens, e_var_type type ) : _name(name), _tokens(tokens), _type(type)
{
}

GrammarVariables::GrammarVariables( const GrammarVariables & src ): _name(src._name), _tokens(src._tokens), _type(src._type)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

GrammarVariables::~GrammarVariables()
{
	
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

GrammarVariables &				GrammarVariables::operator=( GrammarVariables const & rhs )
{
	(void)rhs;
	return *this;
}

std::ostream &			GrammarVariables::print( std::ostream & o) const
{
	o << this->_name << " = ";
	for(std::vector<std::string>::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); it++)
		o << *it << " ";
	return o;
}


std::ostream &			operator<<( std::ostream & o, GrammarVariables const & i )
{
	return i.print(o);
}


/*
** --------------------------------- METHODS ----------------------------------
*/
GrammarVariables *GrammarVariables::generateSubVar(std::size_t index_next_token)
{
	std::vector<std::string> tokens;
	if(index_next_token >= this->_tokens.size())
		return NULL;
	tokens.push_back(this->_tokens[index_next_token]);
	return new GrammarVariables("#" +this->_name, tokens);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::string				GrammarVariables::getName() const
{
	return this->_name;
}

e_var_type				GrammarVariables::getType() const
{
	return this->_type;
}

std::vector<std::string> &GrammarVariables::getTokens()
{
	return this->_tokens;
}

/* ************************************************************************** */