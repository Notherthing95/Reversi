#pragma once
#include <iostream>
#include "board.h"

class COM {
public:
    Move chooseMove(const Board& board, Cell player);
};