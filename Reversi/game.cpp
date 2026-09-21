#include "Game.h"
#include <iostream>

void Game::run() {

    static bool passedLast = false;

    while (true) {
        board.display();

        auto moves = board.legalMoves(currentPlayer);
        if (moves.empty()) {
            std::cout << "\n" << (currentPlayer == Cell::Black ? "黒" : "白")
                << "は置ける場所がありません。パスします\n";

            // 両者パスしたらゲーム終了
            if (passedLast) {
                std::cout << "両者パスしたため、ゲームを終了します\n";
                showResult();
                return;
            }

            passedLast = true;
            switchPlayer();
            continue;
        }

        // 通常の手番処理をする
        passedLast = false;

        if (currentPlayer == Cell::Black) playerTurn();
        else cpuTurn();

        switchPlayer();
    }
}


void Game::switchPlayer() {
    currentPlayer = (currentPlayer == Cell::Black) ? Cell::White : Cell::Black;
}

void Game::playerTurn() {
    std::string input;
    std::cout << "あなた(●)のターンです。数字を縦→横の順で入力してください(例:縦5、横7に置きたい場合57) endで終了\n>";

    while (true) {
        std::cin >> input;

        if (input == "end") {
            std::cout << "endと入力されたため、プログラムを終了します。\n";
            showResult();
            exit(0);

        }

        if (input.size() == 2 &&
            input[0] >= '1' && input[0] <= '8' &&
            input[1] >= '1' && input[1] <= '8') {

            int row = input[0] - '1';
            int line = input[1] - '1';

            if (board.applyMove(row, line, currentPlayer)) break;
        }

        std::cout << "数字以外の入力、もしくは置けない場所の入力です。\n>";
    }
}

void Game::cpuTurn() {
    COM com;
    Move pos = com.chooseMove(board, currentPlayer);

    if (pos.row == -1) {
        std::cout << "COMは置ける場所がありません。パスされます\n";
        return;
    }

    board.applyMove(pos.row, pos.line, currentPlayer);

    std::cout << "COM が (" << pos.row + 1 << ", " << pos.line + 1 << ") に置きました。\n";
}


void Game::showResult() {
    int black = board.countStones(Cell::Black);
    int white = board.countStones(Cell::White);

    std::cout << "\n------ ゲーム終了 ------\n";
    std::cout << "● 黒: " << black << "\n";
    std::cout << "〇 白: " << white << "\n";

    if (black > white) {
        std::cout << "勝者: 黒 あなたの勝ちです YEAHHHHHHHHH!!!!!\n";
    }
    else if (white > black) {
        std::cout << "勝者: 白 あなたの負け！Oh......\n";
    }
    else {
        std::cout << "引き分け Ummmmmm.......\n";
    }
    std::cout << "------------------------\n\n";
}

