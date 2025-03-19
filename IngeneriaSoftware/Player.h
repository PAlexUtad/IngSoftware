#pragma once

#include <windows.h>

#define GetKey(X) (!!(GetAsyncKeyState(X) & 0x8000))  

enum PlayerState 
{
    ST_IDLE,
    ST_JUMPING,
    ST_DUCKING,
    ST_DIVING
};

class Player 
{
private:
    PlayerState state;
    bool isArrowDownPressed;
    bool isBPressed;
    bool shift;

    void printState();

public:
    Player();
    void handleInput();
    void pressB();
    void releaseB();
    void pressArrowDown();
    void releaseArrowDown();
    void update();
};
