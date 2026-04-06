#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 5555

void receiveMessages(SOCKET sock) {
    char buffer[1024];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            cout << "Disconnected from server\n";
            break;
        }

        cout << "\nMessage: " << buffer << endl;
    }
}

int main() {
    WSADATA wsaData;
    SOCKET sock;
    sockaddr_in serverAddr;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2,2), &wsaData);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    thread recvThread(receiveMessages, sock);

    string message;

    while (true) {
        getline(cin, message);
        send(sock, message.c_str(), message.size(), 0);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
