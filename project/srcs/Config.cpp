#include "Config.hpp"
// DEFINE_ENUM(e_type, E_TYPE_ENUM)

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Config::Config(std::string input_file, Node *first_node): _input_file(input_file), _first_node(first_node)
{
}

// Config::Config( const Config & src ): _input_file(src._input_file), _first_node(src._first_node)
// {
// 	(void)src;
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Config::~Config()
{
	if(this->_first_node)
		delete this->_first_node;
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// Config &				Config::operator=( Config const & rhs )
// {
// 	(void)rhs;
// 	return *this;
// }

std::ostream &			operator<<( std::ostream & o, Config const & i )
{
	(void)o;
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/
