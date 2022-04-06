#include "AllConfig.hpp"

void    AllConfig::parse(std::string config_path)
{
    Node *firstNode = Node::digestConfigurationFile(config_path);
    if (!firstNode)
        return ;
    std::list<Node*> server_list = firstNode->getChildrenByFirstName("server");
    
    int total_server = server_list.size();
    std::cout << "Total number of server is: " << total_server << std::endl;



    Node::t_node_list::const_iterator it;
    for(it = server_list.begin(); it != server_list.end(); it++) // ! Loop through each server's config
    {
        ConfigOneServer one_server;
        // one_server.parseDirective(it, "listen");
        // one_server.parseDirective(it, "root");
        // one_server.parseDirective(it, "location");
        one_server.parseDirective(it, "server_name");

        // ! After getting all directives, we add that one_server to _config_all_servers
        _config_all_servers.push_back(one_server);

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