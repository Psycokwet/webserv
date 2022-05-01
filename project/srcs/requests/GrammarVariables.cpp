#include "GrammarVariables.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarVariables::GrammarVariables(std::string name, std::vector<std::string> tokens ) : _name(name), _tokens(tokens)
{
}

GrammarVariables::GrammarVariables( const GrammarVariables & src ): _name(src._name), _tokens(src._tokens)
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


/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::string				GrammarVariables::getName() const
{
	return this->_name;
}

std::vector<std::string> &GrammarVariables::getTokens()
{
	return this->_tokens;
}

/* ************************************************************************** */