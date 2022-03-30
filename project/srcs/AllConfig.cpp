#include "AllConfig.hpp"

void AllConfig::parse_directive_of_one_server(Node* directive_list, t_map_server * one_server, std::string directive_name)
{
    Node::t_node_list sub_directive_list = directive_list->getChildrenByFirstName(directive_name);
    if (sub_directive_list.size() == 0) // ! If there is no directive at that name.
        return ;

    Node::t_node_list ::const_iterator it;
    for(it = sub_directive_list.begin(); it != sub_directive_list.end(); it++) // ! Loop through each element of same directive
    {
        std::cout << **it << std::endl; //it is a Node!!
        std::vector<std::string> inner_args = (**it).get_inner_args(); // vector of string
        std::vector<int>::size_type sz = inner_args.size();
        // std::cout << "inner_args contains:\n";
        for (unsigned i=0; i<sz; i++) // ! Loop through each value of one directive
        {
            // Todo: Get each value of a directive into the map of one_server.
            // Todo: how about location?
            
            
            std::cout << inner_args[i] << "\n";
        }

    }


    

}

void    AllConfig::parse(std::string config_path)
{
    Node *firstNode = Node::digestConfigurationFile(config_path);
    if (!firstNode)
        return ;
    std::list<Node*> server_list = firstNode->getChildrenByFirstName("server");
    
    int total_server = server_list.size();
    std::cout << "Total number of server is: " << total_server << std::endl;



    Node::t_node_list::const_iterator it;
    for(it = server_list.begin(); it != server_list.end(); it++) // ! Loop through each server
    {
        t_map_server one_server;
        parse_directive_of_one_server((*it), &one_server, "listen");
        parse_directive_of_one_server((*it), &one_server, "root");
        parse_directive_of_one_server((*it), &one_server, "cgi");
        parse_directive_of_one_server((*it), &one_server, "location");

        // ! After getting all directives, we add that one_server to _config_all_servers
        _config_all_servers.push_back(one_server);

        t_map_server::const_iterator it_map;
        for (it_map = one_server.begin(); it_map != one_server.end(); it_map++)
        {
            std::cout << it_map->first << " ==> " << it_map->second << std::endl;
        }
    }

    // ! Print the content of _config_all_server



}

std::ostream &			operator<<( std::ostream & o, std::vector <std::string> const & i )
{
	if(i.size() == 0)
		return o;
	for(std::vector <std::string>::const_iterator it = i.begin(); it != i.end(); it++)
		o << *it << " ";
	return o;
}