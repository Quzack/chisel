#include "player.hpp"

using chisel::Player;

Player::Player( int clientFd, int* serverFd ):
    m_clientFd(clientFd),
    m_serverFd(serverFd)
{

}

Player::~Player() {

}

void Player::tick() {
    
}