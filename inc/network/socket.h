#pragma once

#include <winsock2.h>
#include <string>

namespace chisel::sock {
class Client {
public:
    Client ( int );
private:
    const int _fd;
};

class Server {
public:
    Server ();
    ~Server();

    void listen_port( const int );
 
    Client      accept_cl() const;
    int         read_byte() const;
    std::string read_str () const;
private:
    SOCKADDR_IN _addr;
    int         _fd;
};
}