#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned>(time(nullptr)));
    Game game;
    game.run();
    return 0;
}