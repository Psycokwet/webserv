#include "ConfigConsumer.hpp"

/*
** ------------------------------- STATIC -------------------------------------
*/

// LIST_ACTIONS buildList()
// {
// 	LIST_ACTIONS tmp;
// 	tmp.push_back((t_actions_for_key){0, 2, &ConfigConsumer::consume});
// 	return LIST_ACTIONS();
// }

#define ADD_ONE_LEVEL_ACTION(key, actionDefinition, map) { \
	LIST_ACTIONS tmplist; \
	tmplist.push_back(actionDefinition); \
	map[key] = tmplist; \
}

ACTION_MAP ConfigConsumer::initializeActionMap()
{
    ACTION_MAP map;
	ADD_ONE_LEVEL_ACTION("server", ActionForKey(0, 2), map) // ! Action For Key? min = 0, max = 2 for the key "server" meaning: server can be at the deepness < 2 and > 4.
	ADD_ONE_LEVEL_ACTION("listen", ActionForKey(2, 4, "server"), map) // ! min, max is the allowed Deepness level of the directive.
	ADD_ONE_LEVEL_ACTION("server_name", ActionForKey(2, 4, "server"), map) // ! min, max is the allowed Deepness level of the directive.
	ADD_ONE_LEVEL_ACTION("location", ActionForKey(2, 6, "server"), map) 
	ADD_ONE_LEVEL_ACTION("root", ActionForKey(2, 6, "server", "location"), map) // ! min, max is the allowed Deepness level of the directive.
	ADD_ONE_LEVEL_ACTION("index", ActionForKey(2, 6, "server", "location"), map) 
	ADD_ONE_LEVEL_ACTION("autoindex", ActionForKey(2, 6, "server", "location"), map) 
	ADD_ONE_LEVEL_ACTION("error_page", ActionForKey(2, 6, "server", "location"), map) 
	ADD_ONE_LEVEL_ACTION("method", ActionForKey(2, 6, "server", "location"), map) 
	ADD_ONE_LEVEL_ACTION("client_max_body_size", ActionForKey(2, 6, "server", "location"), map); 
	ADD_ONE_LEVEL_ACTION("cgi", ActionForKey(2, 6, "server", "location"), map) 
	ADD_ONE_LEVEL_ACTION("cgi_pass", ActionForKey(2, 6, "server", "location"), map) 
    return map;
}

// Todo: Check valid parents / context


ACTION_MAP ConfigConsumer::_authorize_key_and_actions = ConfigConsumer::initializeActionMap();

	
int ConfigConsumer::isValid(std::string key, int deepness, Node *raw_parents) // ! check the deepness of directive is valid or not
{
	ACTION_MAP::iterator available_actions = ConfigConsumer::_authorize_key_and_actions.find(key);
	if(available_actions == ConfigConsumer::_authorize_key_and_actions.end()) // ! If there is no action key that is predefined, return (false = not supported)
		return false;

	for(LIST_ACTIONS::const_iterator it_list = available_actions->second.begin(); it_list != available_actions->second.end(); it_list++)
	{
		std::cout << "testing... for "<< key << ": at deepness = "<< deepness << std::endl;
		// std::cout << "testing... for Parents Node: "<< *raw_parents << std::endl;
		Node::t_node_map inner_parent_map = raw_parents->getInnerMap();
		std::cout << "inner map: " << inner_parent_map["listen"] << "\n";

		// _inner_map.begin(); it != this->_inner_map.end(); it++)



		if(it_list->isValid(deepness, "add_name_of_parents_here") == true)
			return true;
		std::cout << "error for "<< key << " : "<<deepness<<std::endl;
	}
	return false;
}

int	ConfigConsumer::checkDirectChildrens(Node::t_node_map &childrens)
{
	try
	{
		for(Node::t_node_map ::const_iterator it_map = childrens.begin(); it_map != childrens.end(); it_map++)
		{
			Node::t_node_list &list_childrens = it_map->second->getDirectChildrensList();
			
			// std::cout << "list_children is : " << **(list_childrens.begin()) << std::endl;

			for(Node::t_node_list ::const_iterator it_list = list_childrens.begin(); it_list != list_childrens.end(); it_list++)
			{
				if (ConfigConsumer::checkDirectChildrens((*it_list)->getDirectChildrensMap()) != EXIT_SUCCESS)
					return -EXIT_FAILURE;
			}
			Node::t_inner_args_container &inner_args = it_map->second->getInnerArgs();

			std::cout << "....Key is: " << *(inner_args.begin()) << std::endl;

			if(inner_args.size() != 0 && ConfigConsumer::isValid(*(inner_args.begin()), it_map->second->getDeepness(), it_map->second->getParent()) == false)
				return -EXIT_FAILURE;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return -EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

ConfigConsumer *ConfigConsumer::validateEntry(std::string config_path)
{
	Node *firstNode = Node::digestConfigurationFile(config_path);
	if( !firstNode)
		return NULL;
	// // you can get all servers objects like that :
	if (ConfigConsumer::checkDirectChildrens(firstNode->getDirectChildrensMap()) != EXIT_SUCCESS)
	{
		delete firstNode;
		std::cout << "Invalid configuration file : Some arguments are not supported" << std::endl;
		return NULL;
	}
	// Node::t_node_list servers = firstNode->getChildrenByFirstName("server");
	// for(Node::t_node_list ::const_iterator it = servers.begin(); it != servers.end(); it++)
	// 	std::cout << **it << std::endl; //two * because the list contain pointers to nodes
	// // you can get all listen objects like that too :
	// Node::t_node_list listen_sub_first_server = (*servers.begin())->getChildrenByFirstName("listen");
	// for(Node::t_node_list ::const_iterator it = listen_sub_first_server.begin(); it != listen_sub_first_server.end(); it++)
	// 	std::cout << **it << std::endl; //two * because the list contain pointers to nodes
	return new ConfigConsumer(firstNode);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ConfigConsumer::ConfigConsumer(Node *node) : _node(node)
{
}

ConfigConsumer::ConfigConsumer( const ConfigConsumer & src ) : _node(src._node)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ConfigConsumer::~ConfigConsumer()
{
	if(_node)
		delete this->_node;
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ConfigConsumer &				ConfigConsumer::operator=( ConfigConsumer const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

std::ostream & ConfigConsumer::print(std::ostream & o) const
{
	o << (*this->_node);
	return o;
}

std::ostream &			operator<<( std::ostream & o, ConfigConsumer const & i )
{
	i.print(o);
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void ConfigConsumer::consume(void *accumulator) const//example pointer to function?
{
	(void)accumulator;
	std::cout << "You should replace me by a real implementation !" << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */