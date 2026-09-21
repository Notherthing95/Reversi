#pragma once
#include "Network.h"
#include "board.h"
#include "com.h"
#include <iostream>

class Game {
public:
    Game();
    Game(Network& net, bool isServer);

    void run();
    void runNetwork();

private:
    Board board;
    Cell currentPlayer = Cell::Black;
    Network* network = nullptr;
    bool isMyTurn = false;
    bool requestedExit = false;
    Cell myColor = Cell::Black;

    void playerTurn();
    void cpuTurn();
    bool networkTurn();
    void switchPlayer();
    void showResult();
};

