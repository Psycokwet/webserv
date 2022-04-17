#ifndef CONFIGCONSUMER_HPP
# define CONFIGCONSUMER_HPP

# include <iostream>
# include <string>
# include <map>
# include <list>
# include <sstream>
# include "Node.hpp"
# include "ActionForKey.hpp"
# include "OneServer.hpp"
class ActionForKey;

#define LIST_ACTIONS std::list<ActionForKey>
#define ACTION_MAP std::map<std::string, LIST_ACTIONS>

class ConfigConsumer
{

	public:

		~ConfigConsumer();
		static ConfigConsumer *validateEntry(std::string config_path);
		std::ostream & print(std::ostream & o) const;


		class WrongSyntaxError : public std::exception
        {
            public:
                virtual const char *what() const throw()
				{
					return "Directive has wrong syntax.";
				}
        };

		void consume(void *accumulator) const;

	private:
		static ACTION_MAP _authorize_key_and_actions;	
		Node *_node;

		static int isValid(std::string key, int raw_deepness, Node *raw_parents);

		static int checkDirectChildrens(Node::t_node_map &childrens, void* baseCurrentPointer);
		// static int checkDirectChildrens(Node::t_node_map &childrens);
		static ACTION_MAP initializeActionMap();
		
		// void consume(void *accumulator) const;//example

		ConfigConsumer(Node *node = NULL);
		ConfigConsumer( ConfigConsumer const & src );
		ConfigConsumer &		operator=( ConfigConsumer const & rhs );



};

std::ostream &			operator<<( std::ostream & o, ConfigConsumer const & i );

#endif /* ************************************************** CONFIGCONSUMER_H */