#include "ConfigOneServer.hpp"

ConfigOneServer::ConfigOneServer() //Todo: put default value to each directive
    // _root(NULL),
    // _server_name(NULL),
    // _autoindex(NULL);
    // _location(NULL)
{
    // std::pair<std::string, int> default_addr1("*", 80);
    // std::pair<std::string, int> default_addr2("*", 8000);
    // _listen.push_back(default_addr1);
    // _listen.push_back(default_addr2);

    // _root = "html";
    // _server_name.push_back("");

}


void ConfigOneServer::parseDirective(Node::t_node_list::const_iterator directive_list, std::string directive_name)
{
    Node::t_node_list   directive_sub_one_server = (*directive_list)->getChildrenByFirstName(directive_name);
    if (directive_sub_one_server.size() == 0) // ! If there is no directive at that name
    {
        // std::cout << "Root of server with 0 item: (have default values)\n";
        // std::cout << _root << std::endl;

        // std::cout << "Listen of server with 0 item: (have default values)\n";
        // std::cout << _listen[0].first << " => " << _listen[0].second << "\n";
        // std::cout << _listen[1].first << " => " << _listen[1].second << "\n";

        return ;
    }
    else if (directive_sub_one_server.size() == 1) // There is only one directive at that name
    {
        // std::cout << "Root of server with 1 item:\n";
        // // Todo: parse value to relevant attribute
        // std::cout << **(directive_sub_one_server.begin()) << std::endl;

        std::vector<std::string> inner_args = directive_sub_one_server.front()->get_inner_args();
        // std::cout << "Inner args:\n"; // for example is a vector of string: {root, abc/xyz}, or {listen, 390:2039, server_name}
        // for (unsigned long i = 0; i < inner_args.size(); i++)
        // std::cout << inner_args[i] << std::endl;

        // _listen.clear(); // ! clear the default values
        // Todo: parse inner args into each directive
        this->parseInnerArgs(inner_args, directive_name);
        
        return ;
    }

    else if (directive_sub_one_server.size() > 1) // ! If there is more than one directive in a same server (ex: listen, location ....
    {
        // Todo: parse a list of value of directive
        // std::cout << "Root of server with >1 items:\n";
        // Node::t_node_list::const_iterator it;
        // for (it = directive_sub_one_server.begin(); it != directive_sub_one_server.end(); it++)
        //     std::cout << **(it) << std::endl;

        // std::vector<std::string> inner_args = directive_sub_one_server.back()->get_inner_args();
        // std::cout << "Inner args:\n";
        // for (unsigned long i = 0; i < inner_args.size(); i++)
        // std::cout << inner_args[i] << std::endl;

        // // _listen.clear(); // ! clear the default values
        std::cout << "Can not have more than one directive with the same name in a context\n";
        return ;
    }

}

// # define NAME(x) _##x

void ConfigOneServer::parseInnerArgs(std::vector<std::string> inner_args, std::string directive_name)
{
    _inner_map_one_server.insert( std::pair<std::string, std::vector<std::string> >(directive_name, inner_args) );
}

ConfigOneServer::t_inner_map ConfigOneServer::getInnerMap() const
{
    return (_inner_map_one_server);
}