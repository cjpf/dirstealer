/*
CJ Pfenninger
06/03/2020
www.elearnsecurity.com
Pentest Student Course, eJPT Certification
C++ Programming Lab, Task 1: Create a simple program that steals a user's directory content.
To compile:
    g++ dirstealer.cpp -o dirstealer.exe -lws2_32
*/

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
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

    // Initialize Client Socket


    return 0;
}