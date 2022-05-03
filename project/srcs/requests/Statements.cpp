#include "Statements.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Statements::Statements(int deepness, std::string type, GrammarVariables *gv) : _type(type), _mustBeValid(type == "(" ? true: false), _gv(gv), _deepness(deepness)
{
}

Statements::Statements( const Statements & src ) : _type(src._type), _mustBeValid(src._mustBeValid), _gv(src._gv), _deepness(src._deepness)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Statements::~Statements()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Statements &				Statements::operator=( Statements const & rhs )
{
	(void)rhs;
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}
std::ostream &			Statements::print( std::ostream & o) const
{
	o << this->_type << " : " << this->_gv;
	return o;
}


std::ostream &			operator<<( std::ostream & o, Statements const & i )
{
	return i.print(o);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool Statements::isTheRightClosingStatement(std::string type, GrammarVariables *gv)
{
	if(gv != this->_gv || !((type == ")" && _type == "(")  || (type == "]" && _type == "[")))
		return false;
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */