#pragma once
#include "Board.h"
#include "com.h"
#include <iostream>

class Game {
public:
    void run();

private:
    Board board;
    Cell currentPlayer = Cell::Black;

    void playerTurn();
    void cpuTurn();
    void switchPlayer();
    void showResult();
};
