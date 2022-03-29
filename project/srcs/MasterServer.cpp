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
    this->_allConfig = rhs._allConfig;
    return (*this);
}

MasterServer::~MasterServer()
{}

void    MasterServer::parseConfig(std::string config_path)
{
    _allConfig.parse(config_path);
    
}