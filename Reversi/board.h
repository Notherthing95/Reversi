#pragma once
#include <iostream>
#include <string>
#include <vector>

enum class Cell { None, Black, White };

struct Move {
    int row;
    int line;
};

class Board {
public:
    Board();

    void display() const;

    bool isLegalMove(int row, int line, Cell player) const;
    std::vector<Move> legalMoves(Cell player) const;
    bool applyMove(int row, int line, Cell player);

    Cell getCell(int row, int line) const { return defaultCells[row][line]; }
    int countStones(Cell player) const;

private:
    Cell defaultCells[8][8];
    std::string cells[8][8];

    void updateDisplayCell(int row, int line);
};
