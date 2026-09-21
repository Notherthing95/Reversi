#include "board.h"
#include <iostream>
#include <vector>

Board::Board() {
    for (int row = 0; row < 8; row++) {
        for (int line = 0; line < 8; line++) {
            defaultCells[row][line] = Cell::None;
            updateDisplayCell(row, line);
        }
    }

    // íÜâõÇ…íuÇ≠èâä˙îzíu
    defaultCells[3][3] = Cell::Black;
    defaultCells[3][4] = Cell::White;
    defaultCells[4][3] = Cell::White;
    defaultCells[4][4] = Cell::Black;

    updateDisplayCell(3, 3);
    updateDisplayCell(3, 4);
    updateDisplayCell(4, 3);
    updateDisplayCell(4, 4);
}

void Board::updateDisplayCell(int row, int line) {
    switch (defaultCells[row][line]) {
    case Cell::None:  
        cells[row][line] = "  "; 
        break;

    case Cell::Black: 
        cells[row][line] = "Åú "; 
        break;

    case Cell::White: 
        cells[row][line] = "ÅZ"; 
        break;
    }
}

void Board::display() const {
    std::cout << "   ÇP ÇQ ÇR ÇS ÇT ÇU ÇV ÇW\n";
    std::cout << "  ------------------------\n";
    for (int row = 0; row < 8; row++) {
        std::cout << row + 1 << " ";
        for (int line = 0; line < 8; line++) {
            std::cout << "|" << cells[row][line];
        }
        std::cout << "|\n";
        std::cout << "  ------------------------\n";
    }
}

std::vector<Move> Board::legalMoves(Cell player) const
{
    std::vector<Move> moves;

    for (int row = 0; row < 8; row++) {
        for (int line = 0; line < 8; line++) {
            if (isLegalMove(row, line, player)) {
                moves.push_back({ row, line });
            }
        }
    }

    return moves;
}

bool Board::isLegalMove(int row, int line, Cell player) const {
    if (defaultCells[row][line] != Cell::None) return false;

    Cell opponent = (player == Cell::Black ? Cell::White : Cell::Black);

    const int dr[8] = { -1,-1,-1, 0, 0, 1, 1, 1 };
    const int dl[8] = { -1, 0, 1,-1, 1,-1, 0, 1 };

    for (int i = 0; i < 8; i++) {
        int nr = row + dr[i];
        int nl = line + dl[i];

        if (nr < 0 || nr >= 8 || nl < 0 || nl >= 8) continue;
        if (defaultCells[nr][nl] != opponent) continue;

        while (true) {
            nr += dr[i];
            nl += dl[i];

            if (nr < 0 || nr >= 8 || nl < 0 || nl >= 8) break;
            if (defaultCells[nr][nl] == Cell::None) break;
            if (defaultCells[nr][nl] == player) return true;
        }
    }
    return false;
}


bool Board::applyMove(int row, int line, Cell player) {
    if (!isLegalMove(row, line, player)) 
        return false;

    Cell opponent = (player == Cell::Black ? Cell::White : Cell::Black);

    defaultCells[row][line] = player;

    // ï˚å¸
    const int dr[8] = { -1,-1,-1, 0, 0, 1, 1, 1 };
    const int dl[8] = { -1, 0, 1,-1, 1,-1, 0, 1 };

    for (int i = 0; i < 8; i++) {
        int nr = row + dr[i];
        int nl = line + dl[i];

        if (nr < 0 || nr >= 8 || nl < 0 || nl >= 8)
            continue;
        if (defaultCells[nr][nl] != opponent)
            continue;

        std::vector<std::pair<int, int>> flips;
        flips.push_back({ nr,nl });

        while (true) {
            nr += dr[i];
            nl += dl[i];

            if (nr < 0 || nr >= 8 || nl < 0 || nl >= 8) {
                flips.clear();
                break;
            }
            if (defaultCells[nr][nl] == Cell::None) {
                flips.clear();
                break;
            }
            if (defaultCells[nr][nl] == player) {
                for (auto& p : flips) {
                    defaultCells[p.first][p.second] = player;
                }
                break;
            }

            flips.push_back({ nr, nl });
        }
    }

    // îΩâfÇçXêVÇ∑ÇÈ
    for (int rr = 0; rr < 8; rr++)
        for (int ll = 0; ll < 8; ll++)
            updateDisplayCell(rr, ll);

    return true;
}

int Board::countStones(Cell player) const {
    int count = 0;
    for (int row = 0; row < 8; row++) {
        for (int line = 0; line < 8; line++) {
            if (defaultCells[row][line] == player) count++;
        }
    }
    return count;
}