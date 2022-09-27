#include <thread>

#include "player.hpp"

using chisel::Player;

Player::Player( const int clientFd, const int* serverFd ):
    m_clientFd(clientFd),
    m_serverFd(serverFd),
    m_active  (true)
{

}

Player::~Player() {
    delete m_serverFd;
}

void Player::tick() {
    // TODO 27/9/22: Player packet logic.
}

void Player::disconnect( std::string reason ) {
    // TODO 27/9/22: Send 0x0e packet.
}