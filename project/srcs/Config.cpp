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

int splitAddToNode(std::string &s, Node *current_node)
{
	std::stringstream ss(s);
	std::string tmp_block;
	Node::t_inner_args_container tmp_inner_args;
	while(std::getline(ss, tmp_block, ' '))
	{
		if(tmp_block[0] == '#')
			return EXIT_SUCCESS;
		if(tmp_block == "")
			continue;

		if (tmp_block == "{")
		{
		// std::cout << "GETTING DOWN______" << std::endl;
			if (tmp_inner_args.size() == 0)
				return PARSING_ERROR;
			Node *tmp_node = new Node(LIST, current_node, current_node->getDeepness() + 1, tmp_inner_args);
		// std::cout << "GETTING DOWN______int" << tmp_node << std::endl;
			if(current_node->addNode(tmp_node) != EXIT_SUCCESS)
			{
				delete tmp_node;
				return PARSING_ERROR;
			}
		// std::cout << "GETTING DOWN______int2" << tmp_node << std::endl;
			current_node = tmp_node;
		// std::cout << "GETTING DOWN______\\" <<*current_node<< std::endl;
			//parsing sub object
		}
		else if (tmp_block == "}")
		{
		// std::cout << "GETTING UP______" <<current_node->getDeepness()  << std::endl;
			if((current_node = current_node->getParent()) == NULL)
				return PARSING_ERROR;
		// std::cout << "GETTING UP______\\" << std::endl;
		}else 
		{
			tmp_inner_args.push_back(tmp_block);
		}
		std::cout << tmp_block << ";;;" << std::endl;
		std::cout << " current_node " << *current_node << "::" << std::endl << std::endl;
	}
	std::cout << std::endl;
	return EXIT_SUCCESS;
}

#define PREPARE_AND_SKIP_EMPTY_LIGNES(str) std::replace_if(str.begin(), str.end(), isblank, ' '); \
		trim(str);\
		if(str == "" || str[0] == '#')\
			continue;

int	parseObject(std::ifstream &ifs, std::string tmp_line, Node *current_node)
{
	do {
		PREPARE_AND_SKIP_EMPTY_LIGNES(tmp_line)
		// std::replace_if(tmp_line.begin(), tmp_line.end(), isblank, ' ');
		// trim(tmp_line);
		// if(tmp_line == "" || tmp_line[0] == '#')
		// 	continue;
		if(splitAddToNode(tmp_line, current_node) != EXIT_SUCCESS)
		{
			ifs.close();
			return 0;
		}
	} while(std::getline(ifs, tmp_line));
	return EXIT_SUCCESS;
}

Config *Config::factory(std::string input_file)
{
	Node			first_node(HASHMAP);
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

		if(parseObject(ifs, tmp_line, &first_node) != EXIT_SUCCESS)
		{
			std::cout << "ERROR : " << first_node  << std::endl;
			ifs.close();
			return NULL;
		}
		std::cout <<  "OK : " <<first_node  << std::endl;

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

Config::Config(std::string input_file, Node &first_node): _input_file(input_file), _first_node(first_node)
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
