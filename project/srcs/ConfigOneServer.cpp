#include "ConfigOneServer.hpp"

ConfigOneServer::ConfigOneServer()
:
    _listen(NULL),
    _root(NULL),
    _server_name(NULL),
    _autoindex(NULL);
    _location(NULL)
{}


void ConfigOneServer::parseDirective(std::list<Node*> directive_list, std::string directive_name)
{
    Node::t_node_list   sub_list = directive_list->getChildrenByFirstName(name);
    if (sub_list.size() == 0) // ! If there is no directive at that name.
        return ;
    if (sub_list.size() == 1)
    {
        // Todo: parse value to relevant attribute
    }

    if (sub_list.size() > 1) // ! If there is more than one listen, or location ....

    // Node::t_node_list::const_iterator   it;
    // for(it = sub_list.begin(); it != sub_list.end(); it++) // ! Loop through each element
    // {
    //     std::cout << **it << std::endl; //it is a Node!!
    //     std::vector<std::string> inner_args = (**it).get_inner_args(); // vector of string
    //     std::vector<int>::size_type sz = inner_args.size();
    //     // std::cout << "inner_args contains:\n";
    //     for (unsigned i=0; i<sz; i++) // ! Loop through each value of one directive
    //     {
    //         // Todo: Get each value of a directive into the map of one_server.
    //         // Todo: how about location?
            
            
    //         std::cout << inner_args[i] << "\n";
    //     }

    // }

}