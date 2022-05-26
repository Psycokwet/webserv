#include "ConfigConsumer.hpp"

/*
** ------------------------------- STATIC -------------------------------------
*/

#define ADD_ONE_LEVEL_ACTION(key, actionDefinition, map) { \
	LIST_ACTIONS tmplist; \
	tmplist.push_back(actionDefinition); \
	map[key] = tmplist; \
}

AServerItem *placeholder_consume(Node *node, AServerItem *currentServerItem)
{
	(void)node;
	(void)currentServerItem;
	return NULL;
}

MasterServer* getMasterServerFrom(AServerItem *currentServerItem)
{
	MasterServer *ms = dynamic_cast<MasterServer*>(currentServerItem);
	if (!ms)
		throw ConfigConsumer::UnexpectedStateInConsumer();
	return ms;
}

OneServer* getOneServerFrom(AServerItem *currentServerItem)
{
	OneServer *os = dynamic_cast<OneServer*>(currentServerItem);
	if (!os)
		throw ConfigConsumer::UnexpectedStateInConsumer();
	return os;
}

AServerItem *consumeCreateServer(Node *node, AServerItem *currentServerItem)
{
	MasterServer *ms = getMasterServerFrom(currentServerItem);
	(void)node;
	return ms->createServer();
}
AServerItem *consumeForOneServer(Node *node, AServerItem *currentServerItem)
{
	OneServer *os = getOneServerFrom(currentServerItem);
	os->consume(node);
	return os;
}

ACTION_MAP ConfigConsumer::initializeActionMap()
{
    ACTION_MAP map;
	std::vector<std::string> allowed_parents;

	// allowed_parents.push_back(""); // ! "server" has no parent // Sophie edit -> You don't need that
	// ADD_ONE_LEVEL_ACTION("server", ActionForKey(0, 2, allowed_parents, createServer), map) // ! min = 0, max = 2 for the key "server" meaning: server can be at the deepness < 2 and > 4.
	
	// allowed_parents.push_back("server");
	// ADD_ONE_LEVEL_ACTION("listen", ActionForKey(2, 4, allowed_parents, initListen), map) // ! min, max is the allowed Deepness level of the directive.
	// ADD_ONE_LEVEL_ACTION("location", ActionForKey(2, 6, allowed_parents, initLocation), map) 
	ADD_ONE_LEVEL_ACTION("server", ActionForKey(0, 2, allowed_parents, &consumeCreateServer), map) // ! min = 0, max = 2 for the key "server" meaning: server can be at the deepness < 2 and > 4.
	
	allowed_parents.push_back("server");
	ADD_ONE_LEVEL_ACTION("listen", ActionForKey(2, 4, allowed_parents, &consumeForOneServer), map) // ! min, max is the allowed Deepness level of the directive.
	ADD_ONE_LEVEL_ACTION("location", ActionForKey(2, 6, allowed_parents, &placeholder_consume), map) 
	ADD_ONE_LEVEL_ACTION("server_name", ActionForKey(2, 4, allowed_parents, &placeholder_consume), map)

	// ! setServerName is a method, in ActionForKey class?
	// It can be a method from the configConsumer, it doesn't matter anyways, since it won't use anything private from any class.
	// The only thing that matter, is that it will take a void* to have access to the current server/subobject that need configuration at the moment, cast it right, and then, fill it
	// You could even do basic function pointer here I think
	// ADD_ONE_LEVEL_ACTION("server_name", ActionForKey(2, 4, allowed_parents, setServerName), map) 


	allowed_parents.push_back("location");
	ADD_ONE_LEVEL_ACTION("root", ActionForKey(2, 6, allowed_parents, &placeholder_consume), map)
	ADD_ONE_LEVEL_ACTION("index", ActionForKey(2, 6, allowed_parents, &placeholder_consume), map) 
	ADD_ONE_LEVEL_ACTION("autoindex", ActionForKey(2, 6, allowed_parents, &placeholder_consume), map) 
	ADD_ONE_LEVEL_ACTION("error_page", ActionForKey(2, 6, allowed_parents, &placeholder_consume), map) 
	ADD_ONE_LEVEL_ACTION("method", ActionForKey(2, 6, allowed_parents, &placeholder_consume), map) 
	ADD_ONE_LEVEL_ACTION("client_max_body_size", ActionForKey(2, 6, allowed_parents, &placeholder_consume), map); 
	ADD_ONE_LEVEL_ACTION("cgi", ActionForKey(2, 6, allowed_parents, &placeholder_consume), map) 
	ADD_ONE_LEVEL_ACTION("cgi_pass", ActionForKey(2, 6, allowed_parents, &placeholder_consume), map) 
    return map;
}

// Todo: Check valid parents / context


ACTION_MAP ConfigConsumer::_authorize_key_and_actions = ConfigConsumer::initializeActionMap();

	
AServerItem *ConfigConsumer::consume(int deepness, Node *node, AServerItem *currentServerItem) // ! check the deepness and parent of directive is valid or not
{
	Node::t_inner_args_container &inner_args = node->getInnerArgs(); // ! get all elements of server key (get all servers)
	if (inner_args.size() == 0)
		return currentServerItem; 
	std::string key = *(inner_args.begin());
	std::cout << "... Key is: " << key << std::endl;
	ACTION_MAP::iterator available_actions = ConfigConsumer::_authorize_key_and_actions.find(key); // ! get authorized info for "key / a directive"
	if (available_actions == ConfigConsumer::_authorize_key_and_actions.end()) // ! If there is no action key that is predefined, return (false = not supported)
		return NULL;

	for (LIST_ACTIONS::const_iterator it_list = available_actions->second.begin(); it_list != available_actions->second.end(); it_list++) // ! Loop through each available actions for that key (Ref. initializeActionMap())
	{
		Node *raw_parent = node->getParent()->getParent();
		std::string *parentName = NULL;
		if(raw_parent)
		{
			Node::t_inner_args_container &parent_inner_args = raw_parent->getInnerArgs();
		
			std::cout << "testing... for "<< key << ": at deepness = "<< deepness;
			if (parent_inner_args.size() != 0) // ! if parents is something else than "server"
				parentName = &parent_inner_args[0];
		}
		std::cout << " with parent = " << (parentName ? *parentName : "NO PARENT") << std::endl; // ! parent_inner_args[0] is the parents' name
		if(it_list->isValid(deepness, parentName)) // ! check validity of directive's deepness and its name of parents
			return it_list->consume(node, currentServerItem);
	}
	std::cout << "error for "<< key << " : "<<deepness<<std::endl;
	return NULL; // ! If the deepness and parents is not valid, return (false = wrong Context)
}

int	ConfigConsumer::checkDirectChildrens(Node::t_node_map &childrens, AServerItem* currentServerItem )
{
	try
	{
		for(Node::t_node_map ::const_iterator it_map = childrens.begin(); it_map != childrens.end(); it_map++) // ! Loop through each key (normally have only one key which is "server". It can have more than one key like other directive, but the program should give error because there is only server can be at the first level)
		{
				// throw WrongSyntaxError(); // if it returns null, parsing have met with a terribale fate for some reason, you could alternatively throw an error from parsing itself

			//! Go until the smallest level of directives
			Node::t_node_list &list_childrens = it_map->second->getDirectChildrensList(); 
			for (Node::t_node_list ::const_iterator it_list = list_childrens.begin(); it_list != list_childrens.end(); it_list++)
			{
				AServerItem *nextServerItem = currentServerItem;
				// Doing validity check before going on some under object, for parsing reasons
				if((nextServerItem = ConfigConsumer::consume(it_map->second->getDeepness(), it_map->second, nextServerItem)) == NULL)
					return -EXIT_FAILURE;
				if (ConfigConsumer::checkDirectChildrens((*it_list)->getDirectChildrensMap(), nextServerItem) != EXIT_SUCCESS)
					return -EXIT_FAILURE;
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return -EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

MasterServer *ConfigConsumer::validateEntry(std::string config_path)
{
	Node *firstNode = Node::digestConfigurationFile(config_path);
	if( !firstNode)
		return NULL;

	Node::t_node_map my_map = firstNode->getDirectChildrensMap();

std::cout<<*firstNode<<std::endl;
	// ? To parse config for MasterServer or already build a MasterServer?
	// ! Start off with an empty MasterServer (No child server or any config done there, or default config only if you need some)
	MasterServer *ms = new MasterServer();
	if (ConfigConsumer::checkDirectChildrens(firstNode->getDirectChildrensMap(), ms) != EXIT_SUCCESS)
	{
		delete firstNode;
		delete ms;
		std::cout << "Invalid configuration file : Directives are not supported or in wrong Context." << std::endl;
		return NULL;
	}
	// Node::t_node_list servers = firstNode->getChildrenByFirstName("server");
	// for(Node::t_node_list ::const_iterator it = servers.begin(); it != servers.end(); it++)
	// 	std::cout << **it << std::endl; //two * because the list contain pointers to nodes
	// // you can get all listen objects like that too :
	// Node::t_node_list listen_sub_first_server = (*servers.begin())->getChildrenByFirstName("listen");
	// for(Node::t_node_list ::const_iterator it = listen_sub_first_server.begin(); it != listen_sub_first_server.end(); it++)
	// 	std::cout << **it << std::endl; //two * because the list contain pointers to nodes
	delete firstNode;
	return ms;
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

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */