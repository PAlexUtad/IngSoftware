#include "Player.h"
#include <iostream>

Player::Player() 
    : state(ST_IDLE), isArrowDownPressed(false), isBPressed(false), shift(false) 
{

}

// print the state of the player
void Player::printState() 
{
    switch (state) {
    case ST_IDLE:
        std::cout << "ST_IDLE" << std::endl;
        break;
    case ST_JUMPING:
        std::cout << "ST_JUMPING" << std::endl;
        break;
    case ST_DUCKING:
        std::cout << "ST_DUCKING" << std::endl;
        break;
    case ST_DIVING:
        std::cout << "ST_DIVING" << std::endl;
        break;
    default:
        break;
    }
}

// Check input of the player
void Player::handleInput() 
{
    if (GetKey(0x42) && !isBPressed) {  // 0x42 = 'B' key
        pressB();
    }
    if (!GetKey(0x42) && isBPressed) {
        releaseB();
    }

    if (GetKey(VK_DOWN) && !isArrowDownPressed) {
        pressArrowDown();
    }
    if (!GetKey(VK_DOWN) && isArrowDownPressed) {
        releaseArrowDown();
    }
}

// check if B is pressed
void Player::pressB() 
{
    if (state == ST_IDLE && !isBPressed) {
        state = ST_JUMPING;
        isBPressed = true;
    }
}

// check if B is released
void Player::releaseB() 
{
    if (isBPressed && state == ST_JUMPING) {
        state = ST_IDLE;
        isBPressed = false;
    }
    if (state == ST_DIVING) {
        state = ST_DUCKING;
    }
}

// Check if arrow down is pressed
void Player::pressArrowDown() 
{
    isArrowDownPressed = true;
    if (state == ST_IDLE) {
        state = ST_DUCKING;
    }
    else if (state == ST_JUMPING) {
        state = ST_DIVING;
    }
}

// Check if arrow down is released
void Player::releaseArrowDown() 
{
    isArrowDownPressed = false;
    if (state == ST_DUCKING) {
        state = ST_IDLE;
    }
    if (state == ST_DIVING) {
        state = ST_JUMPING;
    }
}

// update function to show the state
void Player::update()
{
    printState();
}