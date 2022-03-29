#include "AllConfig.hpp"

void AllConfig::parse_directive_of_one_server(Node* directive_list, ConfigOneServer one_server, std::string directive_name)
{
    Node::t_node_list sub_directive_list = directive_list->getChildrenByFirstName(directive_name);
    Node::t_node_list ::const_iterator it;
    std::cout << "\n------" << directive_name << " list of one server: " << std::endl;
    for(it = sub_directive_list.begin(); it != sub_directive_list.end(); it++)
    {
        std::cout << **it << std::endl; //it is a Node!!
        std::vector<std::string> inner_args = (**it).get_inner_args(); // vector of string
        std::vector<int>::size_type sz = inner_args.size();
        // std::cout << "inner_args contains:\n";
        for (unsigned i=0; i<sz; i++)
            std::cout << inner_args[i] << "\n";
        std::cout << '\n';
        // std::cout << (**it).getDirectChildrens()[0] << std::endl;
        // std::size_t pos = (**it).find(directive_name);      // position of "live" in str
        // std::string sub_str = (**it).substr (pos + directive_name.size());     // get from "live" to the end

        // std::cout << sub_str << '\n';

    }

    one_server.parseOneServer();
    

    

}

void    AllConfig::parse(std::string config_path)
{
    Node *firstNode = Node::digestConfigurationFile(config_path);
    if (!firstNode)
        return ;
    std::list<Node*> server_list = firstNode->getChildrenByFirstName("server");
    int total_server = server_list.size();
    std::cout << "Size of server is: " << total_server << std::endl;
    Node::t_node_list::const_iterator it;
    for(it = server_list.begin(); it != server_list.end(); it++)
    {
        ConfigOneServer one_server;
        parse_directive_of_one_server((*it), one_server, "listen");
        parse_directive_of_one_server((*it), one_server, "root");
        parse_directive_of_one_server((*it), one_server, "cgi");
        parse_directive_of_one_server((*it), one_server, "location");
    }

}