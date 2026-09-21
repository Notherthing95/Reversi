#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Network.h"
#include <iostream>
#include <winsock2.h>

Network::Network()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    listenSocket = INVALID_SOCKET;
    connectSocket = INVALID_SOCKET;
    isServer = false;
}

Network::~Network()
{
    Close();
    WSACleanup();
}


bool Network::StartServer(int port)
{
    isServer = true;

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
        return false;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
        return false;

    if (listen(listenSocket, 1) == SOCKET_ERROR)
        return false;

    std::cout << "クライアント接続待ち...\n";

    connectSocket = accept(listenSocket, nullptr, nullptr);
    if (connectSocket == INVALID_SOCKET)
        return false;

    std::cout << "接続されました\n";
    return true;
}

bool Network::ConnectToServer(const char* ip, int port)
{
    isServer = false;

    connectSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connectSocket == INVALID_SOCKET)
        return false;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(connectSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
        return false;

    std::cout << "サーバーに接続しました\n";
    return true;
}


bool Network::Send(const std::string& message)
{
    int result = send(connectSocket, message.c_str(),
        static_cast<int>(message.size()), 0);
    return result > 0;
}

bool Network::SendMove(int row, int col)
{
    unsigned char b[2];

    if (row == -1 && col == -1) { b[0] = 8; b[1] = 8; }         // パス
    else if (row == -2 && col == -2) { b[0] = 9; b[1] = 9; }    // end
    else { b[0] = (unsigned char)row; b[1] = (unsigned char)col; }

    int r = send(connectSocket, (const char*)b, 2, 0);
    return r == 2;
}

static bool RecvAll(SOCKET s, char* buf, int len)
{
    int got = 0;
    while (got < len) {
        int r = recv(s, buf + got, len - got, 0);
        if (r <= 0) return false; // 切断 or エラー
        got += r;
    }
    return true;
}


bool Network::Receive(std::string& message)
{
    char buffer[256]{};
    int result = recv(connectSocket, buffer, sizeof(buffer) - 1, 0);

    if (result <= 0)
        return false;   // 切断 or エラー

    buffer[result] = '\0';
    message = buffer;
    return true;
}

bool Network::ReceiveMove(int& row, int& col)
{
    unsigned char b[2];
    if (!RecvAll(connectSocket, (char*)b, 2)) return false;

    if (b[0] == 8 && b[1] == 8) { row = -1; col = -1; return true; } // パス
    if (b[0] == 9 && b[1] == 9) { row = -2; col = -2; return true; } // end

    row = (int)b[0];
    col = (int)b[1];
    return true;
}



void Network::Close()
{
    if (connectSocket != INVALID_SOCKET)
    {
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
    }

    if (listenSocket != INVALID_SOCKET)
    {
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
    }
}
