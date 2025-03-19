#include <iostream>
#include <conio.h> // for _kbhit() and _getch() (Windows-specific)
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