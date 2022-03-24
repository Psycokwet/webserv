#include "Config.hpp"
// DEFINE_ENUM(e_type, E_TYPE_ENUM)

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

std::string conttoString(Node::t_inner_args_container &cont) 
{
	std::string tmp = "";

	for(Node::t_inner_args_container::const_iterator it = cont.begin(); it != cont.end(); it++)
		tmp = tmp + ";" + *it;
	return tmp;
}
int splitAddToNode(std::string &s, Node **current_node)
{
	std::stringstream ss(s);
	std::string tmp_block;
	Node::t_inner_args_container tmp_inner_args;
	Node *tmp_node = NULL;
	while(std::getline(ss, tmp_block, ' '))
	{
		if(tmp_block[0] == '#')
			return EXIT_SUCCESS;
		if(tmp_block == "")
			continue;

		if (tmp_block == "{")
		{
			if (tmp_inner_args.size() == 0)
				return PARSING_ERROR;
			tmp_node = new Node(NO_TYPE, *current_node, (*current_node)->getDeepness() + 1, tmp_inner_args);
			if(!(*current_node = (*current_node)->addNode(tmp_node)))
			{
				delete tmp_node;
				return PARSING_ERROR;
			}
			return EXIT_SUCCESS;
			//parsing sub object
		}
		else if (tmp_block == "}")
		{
			if((*current_node)->getParent() == NULL)
				return PARSING_ERROR;
			((*current_node) = (*current_node)->getParent()->getParent());
			return EXIT_SUCCESS;
		}else 
		{
			tmp_inner_args.push_back(tmp_block);
		}
	}
	if(tmp_inner_args.size() > 0)
	{
		tmp_node = new Node(NO_TYPE, *current_node, (*current_node)->getDeepness() + 1, tmp_inner_args);
		
		if(!(*current_node)->addNode(tmp_node))
		{
			delete tmp_node;
			return PARSING_ERROR;
		}
	}
	return EXIT_SUCCESS;
}

#define PREPARE_AND_SKIP_EMPTY_LIGNES(str) std::replace_if(str.begin(), str.end(), isblank, ' '); \
		trim(str);\
		if(str == "" || str[0] == '#')\
			continue;

int	parseObject(std::ifstream &ifs, std::string tmp_line, Node **current_node)
{
	do {
		PREPARE_AND_SKIP_EMPTY_LIGNES(tmp_line)
		if(splitAddToNode(tmp_line, current_node) != EXIT_SUCCESS)
		{
			ifs.close();
			return PARSING_ERROR;
		}
	} while(std::getline(ifs, tmp_line));
	return EXIT_SUCCESS;
}

Config *Config::factory(std::string input_file)
{
	Node			*first_node = new Node(HASHMAP);
	Node			*current = first_node;
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

		if(parseObject(ifs, tmp_line, &current) != EXIT_SUCCESS)
		{
			std::cout << "ERROR : " << *first_node  << std::endl;
			ifs.close();
			return NULL;
		}
		std::cout <<  "OK : " <<*first_node  << std::endl;
	}
	ifs.close();
	return new Config(input_file, first_node);
}

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
