#include <thread>
#include <chrono>
#include <iostream>

#include "heart.hpp"
#include "network/http.hpp"
#include "network/packet.hpp"

using chisel::Heart;

using std::to_string;

const std::string CC_HEARTBEAT_URL = "http://www.classicube.net/server/heartbeat";

Heart::Heart( 
    const std::string& configParams, 
    const std::string& salt, 
    const int          playerCount
):
    _url(
        CC_HEARTBEAT_URL + configParams + "&version=" + to_string(chisel::packet::PROTOCOL_VERSION) + "&salt=" + salt + "&users=" + to_string(playerCount)
    )
{
    std::thread(start, this).detach();
}

void Heart::start() const {
    http::Request http { _url };

    const auto res = http.send();
    std::cout << "Server URL: "<< std::string{ res.body.begin(), res.body.end() } << std::endl;

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(45));
        http.send();
    }
}
