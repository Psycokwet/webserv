#ifndef CONFIGCONSUMER_HPP
# define CONFIGCONSUMER_HPP

# include <iostream>
# include <string>
# include <map>
# include <list>
# include <sstream>
# include "Node.hpp"
# include "ActionForKey.hpp"
class ActionForKey;

#define LIST_ACTIONS std::list<ActionForKey>
#define ACTION_MAP std::map<std::string, LIST_ACTIONS>

class ConfigConsumer
{

	public:

		~ConfigConsumer();
		static ConfigConsumer *validateEntry(std::string config_path);
		std::ostream & print(std::ostream & o) const;


	private:
		static int checkDirectChildrens(Node::t_node_map &childrens);
		static int isValid(std::string key, int raw_deepness, Node *raw_parents);
		static ACTION_MAP initializeActionMap();
		static ACTION_MAP _authorize_key_and_actions;	
		
		void consume(void *accumulator) const;//example

		ConfigConsumer(Node *node = NULL);
		ConfigConsumer( ConfigConsumer const & src );
		ConfigConsumer &		operator=( ConfigConsumer const & rhs );


		Node *_node;

};

std::ostream &			operator<<( std::ostream & o, ConfigConsumer const & i );

#endif /* ************************************************** CONFIGCONSUMER_H */