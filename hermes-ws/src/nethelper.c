#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string.h>
#include "nethelper.h"

void InitializeListenSocket(SOCKET* listenSocket, char* port)
{
	WSADATA wsaData;
	SOCKET clientSocket = INVALID_SOCKET;
	int iResult = 0;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // Requests WinSock v2.2

    if (iResult != 0)
    {
        printf("WSAStartup failed! Error code: %d\n", iResult);
        exit(1);
    }

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, port, &hints, &result);

    if (iResult != 0)
    {
        printf("getaddrinfo() failed! Error code: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    *listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (*listenSocket == INVALID_SOCKET)
    {
        printf("Error creating socket. Error code: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    iResult = bind(*listenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR)
    {
        printf("bind() failed! Error code: %d\n", iResult);
        freeaddrinfo(result);
        closesocket(*listenSocket);
        WSACleanup();
        exit(1);
    }

    if (listen(*listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen() failed with error %ld\n", WSAGetLastError());
        closesocket(*listenSocket);
        WSACleanup();
        exit(1);
    }
}