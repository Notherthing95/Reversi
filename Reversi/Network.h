#pragma once
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class Network
{
private:
    SOCKET listenSocket;
    SOCKET connectSocket;
    bool isServer;

public:
    Network();
    ~Network();

    bool StartServer(int port);
    bool ConnectToServer(const char* ip, int port);

    void Close();

    bool Send(const std::string& message);
    bool Receive(std::string& message);

    bool SendMove(int row, int col);// row,colは0-7、パスの場合-1,-1、endの場合-2,-2
    bool ReceiveMove(int& row, int& col);
};

