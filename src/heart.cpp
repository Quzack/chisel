#include <thread>
#include <chrono>
#include <iostream>

#include "heart.hpp"
#include "http_request.hpp"
#include "server.hpp"

using chisel::Heart;

using std::to_string;

const std::string CC_HEARTBEAT_URL = "http://www.classicube.net/server/heartbeat";

Heart::Heart( 
    const std::string& configParams, 
    const std::string& salt, 
    const int          playerCount 
):
    m_url(
        CC_HEARTBEAT_URL + configParams + "&version=7" + to_string(SERVER_VERSION) + "&salt=" + salt + "&users=" + to_string(playerCount)
    )
{
    std::thread(start, this).detach();
    
}

void Heart::start() const {
    std::cout << "Sending heartbeat to: " << m_url << std::endl;
    http::Request http { m_url };

    const auto res = http.send();
    std::cout << "Server URL: "<< std::string{ res.body.begin(), res.body.end() } << std::endl;

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(45));
        http.send();
    }
}
