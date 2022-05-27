#include "AServerItem.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

AServerItem::AServerItem()
{
}

AServerItem::AServerItem( const AServerItem & src )
{
	(void)src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

AServerItem::~AServerItem()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

AServerItem &				AServerItem::operator=( AServerItem const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}


std::ostream &			operator<<( std::ostream & o, AServerItem const & i )
{
	i.print(o);
	return o;
}


std::ostream &			AServerItem::print( std::ostream & o) const
{
	o << "I'm  server item!";
	
	return o;
}
/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */