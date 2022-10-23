#include <winsock2.h>
#include <iostream>

#include "utils.h"
#include "network/packet.h"

namespace chisel::packet {
namespace client {
Identify identify( const chisel::sock::Client& socket ) {
    return {
        socket.read_byte(),
        socket.read_str (),
        socket.read_str (),
        socket.read_byte()
    };
}
}
}