#ifndef CONFIGCONSUMER_HPP
# define CONFIGCONSUMER_HPP

# include <iostream>
# include <string>
# include <map>
# include <list>
# include <sstream>
# include "Node.hpp"
# include "ActionForKey.hpp"
# include "../server/OneServer.hpp"
# include "../server/MasterServer.hpp"
class ActionForKey;

#define LIST_ACTIONS std::list<ActionForKey>
#define ACTION_MAP std::map<std::string, LIST_ACTIONS>

class ConfigConsumer
{

	public:

		~ConfigConsumer();
		static MasterServer *validateEntry(std::string config_path);
		std::ostream & print(std::ostream & o) const;

		class WrongSyntaxError : public std::exception
        {
            public:
                virtual const char *what() const throw()
				{
					return "Directive has wrong syntax.";
				}
        };
		class UnexpectedStateInConsumer : public std::exception
        {
            public:
                virtual const char *what() const throw()
				{
					return "A wrong consumer may have been called, the object given does not hold the right dynamic type.";
				}
        };

	private:
		static ACTION_MAP _authorize_key_and_actions;	
		Node *_node;

		static AServerItem *consume(int deepness, Node *node, AServerItem *currentServerItem);

		static int checkDirectChildrens(Node::t_node_map &childrens, AServerItem* currentServerItem);
		// static int checkDirectChildrens(Node::t_node_map &childrens);
		static ACTION_MAP initializeActionMap();
		
		// void consume(void *accumulator) const;//example

		ConfigConsumer(Node *node = NULL);
		ConfigConsumer( ConfigConsumer const & src );
		ConfigConsumer &		operator=( ConfigConsumer const & rhs );
};

std::ostream &			operator<<( std::ostream & o, ConfigConsumer const & i );

#endif /* ************************************************** CONFIGCONSUMER_H */