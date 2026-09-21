#include "com.h"
#include <cstdlib>  // rand()

Move COM::chooseMove(const Board& board, Cell player)
{
    auto moves = board.legalMoves(player);

    if (moves.empty()) {
        return { -1, -1 };   // ƒpƒX
    }

    int index = rand() % moves.size();

    return { moves[index].row, moves[index].line };
}
