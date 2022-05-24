#include "GrammarVariables.hpp"
DEFINE_ENUM(e_var_type, E_VAR_TYPE_ENUM)

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarVariables::GrammarVariables(std::string name, std::vector<std::string> tokens, e_var_type type) : _name(name), _tokens(tokens), _type(type)
{
}

GrammarVariables::GrammarVariables(const GrammarVariables &src) : _name(src._name), _tokens(src._tokens), _type(src._type)
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

GrammarVariables &GrammarVariables::operator=(GrammarVariables const &rhs)
{
	(void)rhs;
	return *this;
}

std::ostream &GrammarVariables::print(std::ostream &o) const
{
	o << this->_name << " = ";
	for (std::vector<std::string>::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); it++)
		o << *it << " ";

	o << "------_type"
	  << " = " << GetString(this->_type) << " ";
	return o;
}

std::ostream &operator<<(std::ostream &o, GrammarVariables const &i)
{
	return i.print(o);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::string GrammarVariables::getName() const
{
	return this->_name;
}

e_var_type GrammarVariables::getType() const
{
	return this->_type;
}

std::vector<std::string> &GrammarVariables::getTokens()
{
	return this->_tokens;
}

/* ************************************************************************** */