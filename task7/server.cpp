#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 5555

vector<SOCKET> clients;

void broadcastMessage(const string& message, SOCKET sender) {
    for (SOCKET client : clients) {
        if (client != sender) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}

void handleClient(SOCKET clientSocket) {
    char buffer[1024];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            cout << "Client disconnected\n";
            closesocket(clientSocket);
            break;
        }

        string msg(buffer);
        cout << "Client: " << msg << endl;

        broadcastMessage(msg, clientSocket);
    }
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2,2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    cout << "Server started on port " << PORT << endl;

    while (true) {
        clientSocket = accept(serverSocket, nullptr, nullptr);
        clients.push_back(clientSocket);

        thread t(handleClient, clientSocket);
        t.detach();
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}