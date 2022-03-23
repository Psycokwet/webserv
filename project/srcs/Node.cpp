#include "Node.hpp"
DEFINE_ENUM(e_type, E_TYPE_ENUM)

/*
** ---------------------------------- STATIC ----------------------------------
*/

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
	// // o << "Print map content" <<std::endl;
	// std::cout << "shord notde " << this->getParent()<<  std::endl;
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
	// o << "Print list content" <<std::endl;
	for(t_node_list::const_iterator it = this->_inner_list.begin(); it != this->_inner_list.end(); it++)
		(*it)->print(o) << std::endl;
	return o;
}

std::ostream & Node::print_inner_args(std::ostream & o) const
{
	// o << "WILL ??? " << this << std::endl;
	if(this->_inner_args.size() == 0)
		return o;
	// o << "Print inner arguments " << this->inner_args_toString() <<  std::endl;
	for(t_inner_args_container::const_iterator it = this->_inner_args.begin(); it != this->_inner_args.end(); it++)
		o << *it << " ";
		// o << *it <<std::endl;
	return o;
}

std::ostream & Node::print(std::ostream & o) const
{
	// o << "[";
	// o << "node at deepness " << this->_deepness << " has parent :" << this->_parent << " and is " << this << " contains types :" << std::endl;
	// iteratee_typeEnum(this->_type, NULL, &print_type);

	if(HAS_TYPE(_type, HASHMAP)){
		print_inner_args(o);
		print_map(o);
	}
	if(HAS_TYPE(_type, LIST))
		print_list(o);
	// o << "]" << std::endl;

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
			node->addType(LIST);
			this->_inner_map[key] = node;
			return this->_inner_map[key]->addNode(new Node(NO_TYPE, node, node->getDeepness() + 1, node->_inner_args));
		}
		else
		{
			node->addType(HASHMAP);
			if(HAS_TYPE(this->_inner_map[key]->_type, HASHMAP)) return NULL;
			this->_inner_map[key]->_inner_list.push_back(node);
		}
	}else
	{
		node->addType(HASHMAP);
		this->_inner_list.push_back(node);
	}
	return node;
}

void Node::addType(e_type type)
{
	if(this->_type == NO_TYPE)
		this->_type = type;
	else
		this->_type = this->_type | type;
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
int Node::getType() const
{
	return this->_type;
}

/* ************************************************************************** */