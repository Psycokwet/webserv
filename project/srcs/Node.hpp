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
		
		
		Node(e_type type = NO_TYPE, Node* parent = NULL, int deepness = 0, t_inner_args_container _inner_args = t_inner_args_container());
		~Node();
		void addType(e_type type);
		std::ostream & print(std::ostream & o) const;
		Node *getParent() const;
		int getType() const;
		int getDeepness() const;
		bool compareInnerArgs(t_inner_args_container &compare) const;
		std::string inner_args_toString() const;
		Node *addNode(Node *node);
		Node *buildAndAdd(e_type type, Node *parent, int deepness, t_inner_args_container inner_args);

	private:
		Node( Node const & src, Node* parent = NULL);
		Node &		operator=( Node const & rhs );

		// void copy_map(const t_node_map &oldmap, t_node_map &newmap, Node *parent);
		// void copy_list(const t_node_list &oldlist, t_node_list &newlist, Node *parent);

		std::ostream & print_map(std::ostream & o) const;
		std::ostream & print_list(std::ostream & o) const;
		std::ostream & print_inner_args(std::ostream & o) const;

		int _type; //necessary so that we can have multiple types
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