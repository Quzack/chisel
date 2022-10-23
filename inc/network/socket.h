#pragma once

#include <winsock2.h>
#include <string>

namespace chisel::sock {
class Client {
public:
    Client ( int );

    char  read_byte() const;
    char* read_str () const;
private:
    const int _fd;
};

class Server {
public:
    Server ();
    ~Server();

    void listen_port( const int );
 
    Client accept_cl() const;
private:
    SOCKADDR_IN _addr;
    int         _fd;
};
}