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
    this->_configAllServer = rhs._configAllServer;
    return (*this);
}

MasterServer::~MasterServer()
{}
