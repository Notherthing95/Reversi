#include "Game.h"
#include "Network.h"
#include <iostream>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned>(time(nullptr)));

    std::cout << "1:ローカル対戦(VS CPU)\n";
    std::cout << "2:ネット対戦\n> ";

    int mode;
    std::cin >> mode;

    if (mode == 1) {
        Game game;
        game.run();
    }
    else {
        Network net;

        std::cout << "1:サーバ  2:クライアント > ";
        int netMode;
        std::cin >> netMode;

        if (netMode == 1) {
            int port = 55555;
            std::cout << "使用ポート: " << port << "\n";
            net.StartServer(port);

            Game game(net, true);   // サーバ側が自動的に先手となる
            game.runNetwork();
        }
        else {
            std::cin.ignore();

            std::string ip;
            std::string portStr;
            int port;

            std::cout << "IPアドレス(Enterを押すと127.0.0.1): ";
            std::getline(std::cin, ip);
            if (ip.empty()) ip = "127.0.0.1";

            std::cout << "ポート番号 (Enterを押すと55555): ";
            std::getline(std::cin, portStr);
            if (portStr.empty()) port = 55555;
            else port = std::stoi(portStr);

            net.ConnectToServer(ip.c_str(), port);

            Game game(net, false);  // クライアント側が自動的に後手となる
            game.runNetwork();
        }

        net.Close();
    }


    return 0;
}