#pragma once

#include <winsock2.h>
#include <string>
#include <vector>
#include <optional>

namespace chisel::sock {
class Client {
public:
    Client ( int );

    bool operator==(const Client& client) { return this->_fd == client._fd; }

    char         read_byte  ()                           const; // NON BLOCKING
    signed char  read_sbyte ()                           const;
    short        read_short ()                           const;
    signed short read_fshort()                           const;
    std::string  read_str   ()                           const;
    bool         send_pckt  ( const std::vector<char>& ) const;
private:
    int _fd;
};

class Server {
public:
    Server ();
    ~Server();

    void listen_port( const int );
 
    std::optional<Client> accept_cl() const;
private:
    SOCKADDR_IN _addr;
    int         _fd;
};
}