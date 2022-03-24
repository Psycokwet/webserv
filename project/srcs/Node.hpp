#ifndef NODE_HPP
# define NODE_HPP

# include <ctype.h>
# include <iostream>
# include <sstream>
# include <cstring>
# include <list>
# include <map>
# include <vector>
# include <algorithm>
# include <fstream>
# include "../includes/enumFactory.h"
# define E_TYPE_ENUM(XX) \
    XX(NO_TYPE,=0x00000001) \
    XX(HASHMAP,=0x00000010) \
    XX(LIST,=0x00000100) \

DECLARE_ENUM(e_type, E_TYPE_ENUM)
// to play with std::cout << "Value = " << Gete_typeValue(GetString((e_type)1)) << std::endl;

class Node
{
	public:
		typedef std::map<std::string, Node*> t_node_map;
		typedef std::list<Node*> t_node_list;
		typedef std::vector<std::string> t_inner_args_container;
		
		static Node *digestConfigurationFile(std::string input_file);
		static int splitAddToNode(std::string &s, Node **current_node);
		static int parseObject(std::ifstream &ifs, std::string tmp_line, Node **current_node);

		~Node();
		void setType(e_type type);
		std::ostream & print(std::ostream & o) const;
		Node *getParent() const;
		int getType() const;
		int getDeepness() const;
		bool compareInnerArgs(t_inner_args_container &compare) const;
		std::string inner_args_toString() const;
		Node *addNode(Node *node);
		Node *buildAndAdd(e_type type, Node *parent, int deepness, t_inner_args_container inner_args);
		
		t_node_list &getChildrenByFullName(std::string key); //for listen port pwot, it would be ;listen;port;pwot I guess you won't really need this one
		t_node_list getChildrenByFirstName(std::string key); //for listen port pwot, it would be listen
		t_node_map &getDirectChildrens(); // in a server { pwit pwot pwet} objet, your would get the map to pwit pwot pwet, warning ! you will get a list a pwit, a list of pwot and a list of pwet, in case there is multiple occurences of them. They are stored in list by full args contracted. See about "inner_args_toString"

        class IllegalGetterException : public std::exception
        {
            public:
                virtual const char *what() const throw()
				{
					return "You don't have the legal type to get this kind of childrens.";
				}
        };
		class KeyDontExistHereException : public std::exception
        {
            public:
                virtual const char *what() const throw()
				{
					return "The object you are trying to retrieve here are not here.";
				}
        };
	private:
		Node(e_type type = NO_TYPE, Node* parent = NULL, int deepness = 0, t_inner_args_container _inner_args = t_inner_args_container());
		Node( Node const & src, Node* parent = NULL);
		Node &		operator=( Node const & rhs );

		// void copy_map(const t_node_map &oldmap, t_node_map &newmap, Node *parent);
		// void copy_list(const t_node_list &oldlist, t_node_list &newlist, Node *parent);

		std::ostream & print_map(std::ostream & o) const;
		std::ostream & print_list(std::ostream & o) const;
		std::ostream & print_inner_args(std::ostream & o) const;

		e_type _type; //necessary so that we can have multiple types
		Node *_parent;
		int _deepness; //object intrication
		t_inner_args_container _inner_args; //first arg is the identifier of
											//the node server{localhost{node ah que coucou bob}}
											//would have identifier server.localhost.node and
											//would have as args "node" "ah" "que" "coucou" "bob"

		t_node_map _inner_map;
		t_node_list _inner_list;

};

std::ostream &			operator<<( std::ostream & o, Node const & i );

#endif /* ********************************************************** NODE_H */