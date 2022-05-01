#include "GrammarParserBuilderMarker.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

GrammarParserBuilderMarker::GrammarParserBuilderMarker(int deepness) :_deepness(deepness)
{
}

GrammarParserBuilderMarker::GrammarParserBuilderMarker( const GrammarParserBuilderMarker & src ) :_deepness(src._deepness)
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
	o << this->_deepness;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */