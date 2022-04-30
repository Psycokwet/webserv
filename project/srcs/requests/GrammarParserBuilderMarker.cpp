#include "GrammarParserBuilderMarker.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarParserBuilderMarker::GrammarParserBuilderMarker()
{
}

GrammarParserBuilderMarker::GrammarParserBuilderMarker( const GrammarParserBuilderMarker & src )
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


std::ostream &			GrammarVariables::print( std::ostream & o) const
{
	o << this->_name << " = ";
	for(std::vector<std::string>::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); it++)
		o << *it << " ";
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */