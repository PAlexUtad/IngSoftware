#include <iostream>
#include "Player.h"


int main() 
{
    Player player;

    while (true) //escape condition?
    {
        player.handleInput();
        player.update();
    }

    return 0;
}