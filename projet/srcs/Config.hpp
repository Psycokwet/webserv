#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <string>
# include <list>
# include <map>
# include <algorithm>
# include <fstream>
# include "../includes/enumFactory.h"
# define E_TYPE_ENUM(XX) \
    XX(NO_TYPE,=0x00000001) \
    XX(HASHMAP,=0x00000010) \
    XX(LIST,=0x00000100) \
    XX(ENDVALUE_STD_ARRAY,=0x00001000) \
    XX(ENDVALUE_KEYWORD_ONLY,=0x00010000) \

DECLARE_ENUM(e_type, E_TYPE_ENUM)

class Config
{

	public:

		~Config();
		static Config *factory(std::string input_file);
		

	private:

		class Node
		{
			public:
				typedef std::map<std::string, Node> t_node_map;
				typedef std::list<Node> t_node_list;

				// enum e_type {
				// 	NO_TYPE = 0x00000000,
				// 	HASHMAP = 0x00000001,
				// 	LIST = 0x00000010,
				// 	ENDVALUE_STD_ARRAY = 0x00000100,
				// 	ENDVALUE_KEYWORD_ONLY = 0x00001000
				// };

				Node(e_type type = NO_TYPE, Node* parent = NULL);
				Node( Node const & src, Node* parent = NULL);
				~Node();
				void addType(e_type type);
				std::ostream & print(std::ostream & o) const;

			private:
				Node &		operator=( Node const & rhs );

				void copy_map(const t_node_map &oldmap, t_node_map &newmap, Node *parent);
				void copy_list(const t_node_list &oldlist, t_node_list &newlist, Node *parent);

				std::ostream & print_map(std::ostream & o) const;
				std::ostream & print_list(std::ostream & o) const;

				int _type; //necessary so that we can have multiple types
				Node *_parent;
				std::string _inner_value;

				t_node_map _inner_map;
				t_node_list _inner_list;

		};

		Config(std::string input_file = "", Config::Node first_node = Config::Node());
		Config( Config const & src );
		Config &		operator=( Config const & rhs );

		std::string _input_file;
		Config::Node _first_node;


};

std::ostream &			operator<<( std::ostream & o, Config const & i );

#endif /* ********************************************************** CONFIG_H */