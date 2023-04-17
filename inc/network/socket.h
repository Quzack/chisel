#pragma once

#include <winsock2.h>
#include <string>
#include <vector>

namespace chisel::sock {
class Client {
public:
    Client ( int );

    char read_byte      ()                           const; // NON BLOCKING
    std::string read_str()                           const;
    bool send_pckt      ( const std::vector<char>& ) const;
private:
    int _fd;
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