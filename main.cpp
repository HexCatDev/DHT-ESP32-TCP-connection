#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;
#pragma comment(lib, "ws2_32.lib")

int main() {

    string ipAddress;
    int port;
    cout << "Enter IP address: ";
    cin >> ipAddress;  
    cout << "Enter port number: ";
    cin >> port;
    cout << "IP address: " << ipAddress << endl;
    cout << "Port number: " << port << endl;

    //Holds server data
    sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &server.sin_addr);
    //end of struct

    //start Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    cout << "WSAStartup failed!" << endl;
    return 1;
    }

    //create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);


    if (sock == INVALID_SOCKET) {
        cout << "Error creating socket!" << endl;
        WSACleanup();
        return 1;
    }
    //connect to server (ESP32) + chceck for error
    int result = connect(sock, (sockaddr*)&server, sizeof(server));
    for(int i = 0; i < 10 && result == SOCKET_ERROR; i++) {
        cout << "Trying to connect: " << i+1 << endl;
        cout << "passes:" << i << endl;
        if (result == SOCKET_ERROR) {
            cout << "Failed to connect!" << endl;
        }
        else {
            cout << "Connected!" << endl;
        }
        sleep_for(milliseconds(500));
    }
    char buffer[512];
    cout << "press anykey to exit" << endl;
    cout << endl << "running " << endl << "******************************" << endl;

    while(true)
    {   
        if (_kbhit()) break;
        int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            cout <<buffer << endl;
        }
        else if (bytesReceived == 0) {
            cout << "Connection closed by server." << endl;
            break;
        }
        else {
            cout << "recv failed with error: " << WSAGetLastError() << endl;
            break;
        }
    }
    closesocket(sock);
    WSACleanup();
}