#include <thread>
#include <chrono>

#include "heart.hpp"

using chisel::Heart;

const std::string CC_HEARTBEAT_URL = "www.classicube.net/server/heartbeat";

Heart::Heart( 
    const std::string& configParams, 
    const std::string& salt, 
    const int          playerCount 
):
    m_url(CC_HEARTBEAT_URL + configParams + "&version=7&salt=" + salt + "&users=" + std::to_string(playerCount))
{
    std::thread(start, this).detach();
}

void Heart::start() const {
    while(true) {
        pulse();
        std::this_thread::sleep_for(std::chrono::seconds(45));
    }
}

void Heart::pulse() const {
    // TODO:
    // - Build URL to ClassiCube
    // - Send GET request
}