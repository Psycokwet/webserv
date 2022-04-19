#include "Node.hpp"
DEFINE_ENUM(e_type, E_TYPE_ENUM)

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
int Node::splitAddToNode(std::string &s, Node **current_node)
{
	std::string tmp_block;
	Node::t_inner_args_container tmp_inner_args;
	Node *tmp_node = NULL;
	if(s.size() > 0 && s.at(s.size() - 1) == ';') s.resize(s.size() - 1);
	std::stringstream ss(s);
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

int	Node::parseObject(std::ifstream &ifs, std::string tmp_line, Node **current_node)
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

Node *Node::digestConfigurationFile(std::string input_file)
{
	std::ifstream	ifs(input_file.c_str());
	std::string		tmp_line = "";

	if (!ifs.is_open())
	{
		std::cout << "File innaccessible or non existing" << std::endl;
		ifs.close();
		return NULL;
	}
	Node			*first_node = new Node(HASHMAP);
	Node			*current = first_node;
	while(std::getline(ifs, tmp_line))
	{
		PREPARE_AND_SKIP_EMPTY_LIGNES(tmp_line)

		if(parseObject(ifs, tmp_line, &current) != EXIT_SUCCESS)
		{
			ifs.close();
			std::cout << "The configuration file contains syntax errors" << std::endl;
			return NULL;
		}
	}
	ifs.close();
	if(current != first_node)
	{
		delete first_node;
		std::cout << "The configuration file contains syntax errors" << std::endl;
		return NULL;
	}
	return first_node;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Node::Node(e_type type, Node *parent, int deepness, t_inner_args_container inner_args) : _type(type), _parent(parent), _deepness(deepness), _inner_args(inner_args)
{
}

// void Node::copy_map(const t_node_map &oldmap, t_node_map &newmap, Node *parent)
// {
// 	std::cout<<"hash"<<std::endl;
// 	for(t_node_map::const_iterator it = oldmap.begin(); it != oldmap.end(); it++)
// 		newmap[it->first] = Node(it->second, parent);
// }

// void Node::copy_list(const t_node_list &oldlist, t_node_list &newlist, Node *parent)
// {
// 	std::cout<<"list"<<std::endl;
// 	for(t_node_list::const_iterator it = oldlist.begin(); it != oldlist.end(); it++)
// 		newlist.push_back(Node(*it, parent));
// }


// Node::Node( const Node &src, Node *parent): _type(src._type), _parent(parent), _inner_args(t_inner_args_container(src._inner_args))
// {
// 	if(HAS_TYPE(_type, HASHMAP))
// 		copy_map(src._inner_map, this->_inner_map, this);
// 	if(HAS_TYPE(_type, LIST))
// 		copy_list(src._inner_list, this->_inner_list, this);
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/


Node::~Node()
{
	if(HAS_TYPE(_type, HASHMAP))
		for(t_node_map::const_iterator it = this->_inner_map.begin(); it != this->_inner_map.end(); it++)
			delete it->second;
	if(HAS_TYPE(_type, LIST))
		for(t_node_list::const_iterator it = this->_inner_list.begin(); it != this->_inner_list.end(); it++)
			delete (*it);
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// Node &				Node::operator=( Node const & rhs )
// {
// 	(void)rhs;
// 	return *this;
// }

void print_type(e_type value, void* raw)
{
	(void)raw;
	std::cout << "\t" << GetString(value) << std::endl;
}


std::ostream & Node::print_map(std::ostream & o) const
{
	if(this->_inner_map.size() == 0)
		return o;
	if(this->_parent)
		o << "{" <<std::endl;
	for(t_node_map::const_iterator it = this->_inner_map.begin(); it != this->_inner_map.end(); it++)
		it->second->print(o);
	if(this->_parent)
		o << "}" <<std::endl;
	return o;
}
std::ostream & Node::print_list(std::ostream & o) const
{
	if(this->_inner_list.size() == 0)
		return o;
	for(t_node_list::const_iterator it = this->_inner_list.begin(); it != this->_inner_list.end(); it++)
		(*it)->print(o) << std::endl;
	return o;
}

std::ostream & Node::print_inner_args(std::ostream & o) const
{
	if(this->_inner_args.size() == 0)
		return o;
	for(t_inner_args_container::const_iterator it = this->_inner_args.begin(); it != this->_inner_args.end(); it++)
		o << *it << " ";
	return o;
}

std::ostream & Node::print(std::ostream & o) const
{
	if(HAS_TYPE(_type, HASHMAP)){
		print_inner_args(o);
		print_map(o);
	}
	if(HAS_TYPE(_type, LIST))
		print_list(o);

	return o;
}

std::ostream &			operator<<( std::ostream & o, Node const & i )
{
	i.print(o);
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

Node *Node::addNode(Node *node)
{
	std::string key = node->inner_args_toString();
	if(HAS_TYPE(_type, HASHMAP))
	{
		if(this->_inner_map.find(key) == this->_inner_map.end())
		{
			node->setType(LIST);
			this->_inner_map[key] = node;
			return this->_inner_map[key]->addNode(new Node(NO_TYPE, node, node->getDeepness() + 1, node->_inner_args));
		}
		else
		{
			node->_parent = this->_inner_map[key];
			node->_deepness = this->_inner_map[key]->_deepness + 1;
			node->setType(HASHMAP);
			if(HAS_TYPE(this->_inner_map[key]->_type, HASHMAP)) return NULL;
			this->_inner_map[key]->_inner_list.push_back(node);
		}
	}else
	{
		node->setType(HASHMAP);
		this->_inner_list.push_back(node);
	}
	return node;
}

void Node::setType(e_type type)
{
	if(this->_type != NO_TYPE)
		return;//should throw an error... But honestly, should not happens so...
	this->_type = type;
}

bool Node::compareInnerArgs(t_inner_args_container &compare) const
{
	unsigned int i = 0;
	if(this->_inner_args.size() != compare.size())
		return false;
	while (compare.size() > i)
	{
		if(this->_inner_args[i] != compare[i])
			return false;
		i++;
	}
	return true;
}

std::string Node::inner_args_toString() const
{
	std::string tmp = "";

	for(t_inner_args_container::const_iterator it = this->_inner_args.begin(); it != this->_inner_args.end(); it++)
		tmp = tmp + ";" + *it;
	return tmp;
}

Node::t_node_list &Node::getChildrenByFullName(std::string key)
{
	if(HAS_TYPE(this->_type, LIST))
		throw IllegalGetterException();

	if(this->_inner_map.find(key) == this->_inner_map.end())
		throw KeyDontExistHereException();
	return this->_inner_map[key]->_inner_list;
}

Node::t_node_list Node::getChildrenByFirstName(std::string key)
{
	if(HAS_TYPE(this->_type, LIST))
		throw IllegalGetterException();
	t_node_list tmp;
	for(t_node_map::const_iterator it = this->_inner_map.begin(); it != this->_inner_map.end(); it++)
		if(it->second->_inner_args[0] == key)
			copy(it->second->_inner_list.begin(), it->second->_inner_list.end(), std::back_inserter(tmp));
	return tmp;
}

Node::t_node_list &Node::getDirectChildrensList()
{
	if(HAS_TYPE(this->_type, LIST))
		return this->_inner_list;
	throw IllegalGetterException();
}

Node::t_node_map &Node::getDirectChildrensMap()
{
	if(HAS_TYPE(this->_type, HASHMAP))
		return this->_inner_map;
	throw IllegalGetterException();
}

Node::t_inner_args_container Node::get_inner_args()
{
	return (_inner_args);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/
Node *Node::getParent() const
{
	return this->_parent;
}

int Node::getDeepness() const
{
	return this->_deepness;
}
e_type Node::getType() const
{
	return this->_type;
}

Node::t_inner_args_container &Node::getInnerArgs()
{
	return this->_inner_args;
}

// Node::t_node_map &Node::getInnerMap()
// {
// 	return this->_inner_map;
// }
/* ************************************************************************** */