#include "MasterServer.hpp"

MasterServer::MasterServer()
{}

MasterServer::MasterServer(const MasterServer & src)
{
    if (this != &src)
        *this = src;
}

MasterServer & MasterServer::operator=(const MasterServer & rhs)
{
    return (*this);
}

MasterServer::~MasterServer()
{}

void    MasterServer::parseConfig(std::string config_path)
{
    // _firstNode = Node::digestConfigurationFile(config_path);
    // if (!_firstNode)
    //     return ;
    // _serverList = _firstNode->getChildrenByFirstName("server");
}

int     MasterServer::setup()
{


}