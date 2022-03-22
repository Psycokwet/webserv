#include "Config.hpp"
DEFINE_ENUM(e_type, E_TYPE_ENUM)

/* ******************************CONFIG************************************** */

/*
** ---------------------------------- STATIC ----------------------------------
*/

void ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(isblank))));
}

void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(isblank))).base(), s.end());
}

void trim(std::string &s)
{
	ltrim(s);
	rtrim(s);
}

#define PARSING_ERROR -1

int Config::splitAddToNode(std::string &s, Config::Node *current_node)
{
	std::stringstream ss(s);
	std::string tmp_block;
	while(std::getline(ss, tmp_block, ' '))
	{
		if(tmp_block[0] == '#')
			return EXIT_SUCCESS;
		if(tmp_block == "")
			continue;

		if (tmp_block == "{")
		{
			current_node.addType(HASHMAP);
			//parsing sub object
		}
		else if (tmp_block == "}")
		{
			if((current_node = current_node.getParent()) == NULL)
				return PARSING_ERROR;
		}else 
		{

		}
		std::cout << tmp_block << ";";
	}
	std::cout << std::endl;
	return EXIT_SUCCESS;
}

#define PREPARE_AND_SKIP_EMPTY_LIGNES(str) std::replace_if(str.begin(), str.end(), isblank, ' '); \
		trim(str);\
		if(str == "" || str[0] == '#')\
			continue;

int	Config::parseObject(std::ifstream ifs, std::string tmp_line, Config::Node *current_node)
{
	do
	{
		PREPARE_AND_SKIP_EMPTY_LIGNES(tmp_line)
		// std::replace_if(tmp_line.begin(), tmp_line.end(), isblank, ' ');
		// trim(tmp_line);
		// if(tmp_line == "" || tmp_line[0] == '#')
		// 	continue;
		if(splitAddToNode(tmp_line, current_node) != EXIT_SUCCESS)
		{
			ifs.close();
			return NULL;
		}
	} while(std::getline(ifs, tmp_line));
	return EXIT_SUCCESS;
}

Config *Config::factory(std::string input_file)
{
	Config::Node	first_node;
	std::ifstream	ifs(input_file.c_str());
	std::string		tmp_line = "";

	if (!ifs.is_open())
	{
		std::cout << "File innaccessible or non existing" << std::endl;
		ifs.close();
		return NULL;
	}
	while(std::getline(ifs, tmp_line))
	{
		PREPARE_AND_SKIP_EMPTY_LIGNES(tmp_line)
		// std::replace_if(tmp_line.begin(), tmp_line.end(), isblank, ' ');
		// trim(tmp_line);
		// if(tmp_line == "" || tmp_line[0] == '#')
		// 	continue;
		if(parseObject(ifs, tmp_line, &first_node)) != EXIT_SUCCESS)
		{
			ifs.close();
			return NULL;
		}
		// if(split(tmp_line, current_node) != EXIT_SUCCESS)
		// {
		// 	ifs.close();
		// 	return NULL;
		// }
	}
	ifs.close();
	return new Config(input_file, first_node);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Config::Config(std::string input_file, Config::Node first_node): _input_file(input_file), _first_node(first_node)
{
}

Config::Config( const Config & src )
{
	(void)src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Config::~Config()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Config &				Config::operator=( Config const & rhs )
{
	(void)rhs;
	return *this;
}

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


/* ******************************NODE**************************************** */



/*
** ---------------------------------- STATIC ----------------------------------
*/

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Config::Node::Node(e_type type, Config::Node *parent) : _type(type), _parent(parent)
{
}

void Config::Node::copy_map(const t_node_map &oldmap, t_node_map &newmap, Config::Node *parent)
{
	std::cout<<"hash"<<std::endl;
	for(t_node_map::const_iterator it = oldmap.begin(); it != oldmap.end(); it++)
		newmap[it->first] = Node(it->second, parent);
}
void Config::Node::copy_list(const t_node_list &oldlist, t_node_list &newlist, Config::Node *parent)
{
	std::cout<<"list"<<std::endl;
	for(t_node_list::const_iterator it = oldlist.begin(); it != oldlist.end(); it++)
		newlist.push_back(Node(*it, parent));
}


Config::Node::Node( const Config::Node &src, Config::Node *parent): _type(src._type), _parent(parent), _inner_args(std::list<std::string>(src._inner_args))
{
	if(HAS_TYPE(_type, HASHMAP))
		copy_map(src._inner_map, this->_inner_map, this);
	if(HAS_TYPE(_type, LIST))
		copy_list(src._inner_list, this->_inner_list, this);
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Config::Node::~Node()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Config::Node &				Config::Node::operator=( Config::Node const & rhs )
{
	(void)rhs;
	return *this;
}

void print_type(e_type value, void* raw)
{
	(void)raw;
	std::cout << "\t" << GetString(value) << std::endl;
}


std::ostream & Config::Node::print_map(std::ostream & o) const
{
	std::cout<<"Printf map content"<<std::endl;
	for(t_node_map::const_iterator it = this->_inner_map.begin(); it != this->_inner_map.end(); it++)
		it->second.print(o);
	return o;
}
std::ostream & Config::Node::print_list(std::ostream & o) const
{
	std::cout<<"Printf list content"<<std::endl;
	for(t_node_list::const_iterator it = this->_inner_list.begin(); it != this->_inner_list.end(); it++)
		it->print(o);
	return o;
}

std::ostream & Config::Node::print(std::ostream & o) const
{
	o << "Contains types :" << std::endl;
	iteratee_typeEnum(this->_type, NULL, &print_type);
	if(HAS_TYPE(_type, HASHMAP))
		print_map(o);
	if(HAS_TYPE(_type, LIST))
		print_list(o);


	// for ()

	// std::cout << "Value = " << Gete_typeValue(GetString((e_type)1)) << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void Config::Node::addType(e_type type)
{
	this->_type = this->_type | type;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
Config::Node *Config::Node::getParent() const
{
	return this->_parent
}


/* ************************************************************************** */