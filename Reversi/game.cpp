#include "Game.h"
#include <iostream>

Game::Game() {}

Game::Game(Network& net, bool isServer)
{
    network = &net;
    isMyTurn = isServer;
    myColor = isServer ? Cell::Black : Cell::White;
}


void Game::runNetwork()
{
    bool passedLast = false;

    while (true) {
        board.display();

        if (isMyTurn) {
            //置ける場所のチェック(自ターンのみ)
            auto moves = board.legalMoves(currentPlayer);
            if (moves.empty()) {
                std::cout << "\nあなたは置ける場所がありません。パスします\n";
                network->SendMove(-1, -1);

                if (passedLast) {
                    std::cout << "両者パスのため終了\n";
                    showResult();
                    return;
                }

                passedLast = true;
                switchPlayer();
                isMyTurn = false;   // 相手の番
                continue;
            }

            // 通常手
            passedLast = false;
            playerTurn();

            if (requestedExit) {
                board.display();
                showResult();
                return;
            }

            switchPlayer();
            isMyTurn = false;

        }
        else {
            // 相手のターン
            bool opponentPassed = networkTurn();
            if (opponentPassed) {
                if (passedLast) {
                    std::cout << "両者パスのため終了\n";
                    showResult();
                    return;
                }
                passedLast = true;
            }
            else {
                passedLast = false;
            }

            switchPlayer();
            isMyTurn = true;   // 自分の番
        }
    }
}





bool Game::networkTurn()
{
    int row, col;

    std::cout << "相手の手を待っています...\n";

    if (!network->ReceiveMove(row, col)) {
        std::cout << "通信が切断されました。\n";
        showResult();
        std::exit(0);
    }

    // ★ここに追加（最優先）
    if (row == -2 && col == -2) {
        std::cout << "相手が終了しました。\n";
        showResult();
        std::exit(0);
    }

    // パス
    if (row == -1 && col == -1) {
        std::cout << "相手はパスしました。\n";
        return true;
    }

    // 通常手
    if (!board.applyMove(row, col, currentPlayer)) {
        std::cout << "ERROR!! 不正な手: (" << row + 1 << "," << col + 1 << ")\n";
        showResult();
        std::exit(0);
    }

    std::cout << "相手が (" << row + 1 << ", " << col + 1 << ") に置きました。\n";
    return false;
}





void Game::run() {

    static bool passedLast = false;

    while (true) {
        board.display();

        auto moves = board.legalMoves(currentPlayer);
        if (moves.empty()) {
            std::cout << "\n" << (currentPlayer == Cell::Black ? "黒" : "白")
                << "は置ける場所がありません。パスします\n";

            if (passedLast) {
                std::cout << "両者パスしたため、ゲームを終了します\n";
                showResult();
                return;
            }

            passedLast = true;
            switchPlayer();
            continue;
        }

        passedLast = false;

        if (currentPlayer == Cell::Black) playerTurn();
        else cpuTurn();

        if (requestedExit) {
            showResult();
            return;
        }

        switchPlayer();
    }
}



void Game::switchPlayer() {
    currentPlayer = (currentPlayer == Cell::Black) ? Cell::White : Cell::Black;
}

void Game::playerTurn() {
    std::string input;
    const char* mark = (currentPlayer == Cell::Black) ? "●" : "〇";
    std::cout << "あなた(" << mark << ")のターンです。数字を縦→横の順で入力してください(例:縦5、横7に置きたい場合57) endで終了\n>";

    while (true) {
        std::cin >> input;

        if (input == "end") {
            std::cout << "endと入力されたため、ゲームを終了します。\n";
            requestedExit = true;
            if (network) network->SendMove(-2, -2);
            return;
        }



        if (input.size() == 2 &&
            input[0] >= '1' && input[0] <= '8' &&
            input[1] >= '1' && input[1] <= '8') {

            int row = input[0] - '1';
            int line = input[1] - '1';

            if (board.applyMove(row, line, currentPlayer)) {
                if (network) {
                    network->SendMove(row, line);
                }
                break;
            }

        }

        std::cout << "数字以外の入力、もしくは置けない場所の入力です。\n>";
    }
}

void Game::cpuTurn() {
    COM com;
    Move pos = com.chooseMove(board, currentPlayer);

    if (pos.row == -1) {
        std::cout << "COMは置ける場所がありません。パスされました\n";
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

    Cell winner;
    bool draw = false;

    if (black > white) winner = Cell::Black;
    else if (white > black) winner = Cell::White;
    else draw = true;

    if (draw) {
        std::cout << "引き分け Ummmmmm.......\n";
    }
    else {
        const char* winnerText = (winner == Cell::Black) ? "黒(●)" : "白(〇)";
        std::cout << "勝者: " << winnerText << "\n";

        if (winner == myColor) {
            std::cout << "あなたの勝ちです YEAHHHHHHHHH!!!!!\n";
        }
        else {
            std::cout << "あなたの負け！Oh......\n";
        }
    }

    std::cout << "------------------------\n\n";
}

