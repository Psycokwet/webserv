#ifndef LISTEN_HPP
#define LISTEN_HPP

#include <string>

class Listen
{
    public:
        Listen();
        ~Listen();

        std::string getAddress() const;
        int getPort() const;
        std::string getDefaultServer() const;

    private:
        std::string     _host;
        int             _port;
        std::string     _default_server;
};

#endif