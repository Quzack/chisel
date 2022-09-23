#include <thread>
#include <chrono>

#include "heart.hpp"

using chisel::Heart;

Heart::Heart( 
    const Config*      config, 
    const std::string& salt, 
    const int          playerCount 
):
    m_config     (config),
    m_salt       (salt),
    m_playerCount(playerCount)
{
    std::thread(start, this).detach();
}

Heart::~Heart() {
    delete m_config;
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