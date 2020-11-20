#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "constants.h"

#pragma comment(lib, "Ws2_32.lib")

void RunWebServer()
{
    WSADATA wsaData;
    int iResult = 0;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

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

    iResult = getaddrinfo(NULL, PORT, &hints, &result);

    if (iResult != 0)
    {
        printf("getaddrinfo() failed! Error code: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    SOCKET listenSocket = INVALID_SOCKET;
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (listenSocket == INVALID_SOCKET)
    {
        printf("Error creating socket. Error code: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    iResult = bind(listenSocket, result->ai_addr, (int) result->ai_addrlen);

    if (iResult == SOCKET_ERROR)
    {
        printf("bind() failed! Error code: %d\n", iResult);
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen() failed with error %ld\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    SOCKET clientSocket = INVALID_SOCKET;

    printf("Web Server started! Listening on port %s\n", PORT);

    while (1)
    {
        clientSocket = accept(listenSocket, NULL, NULL);

        if (clientSocket == INVALID_SOCKET)
        {
            printf("accept() failed: %d\n", WSAGetLastError());
            closesocket(listenSocket);
            WSACleanup();
            exit(1);
        }

        int bufLen = 1024;
        char recvBuf[1024];

        do
        {
            iResult = recv(clientSocket, recvBuf, bufLen, 0);
            if (iResult > 0)
            {
                // printf("%s\n", recvBuf);
                int iSendResult = send(clientSocket, staticResponse, (int) strlen(staticResponse), 0);

                if (iSendResult == SOCKET_ERROR)
                {
                    printf("send() failed! Error code: %d\n", WSAGetLastError());
                    closesocket(clientSocket);
                    WSACleanup();
                    exit(1);
                }

                closesocket(clientSocket);
            }
        } while (iResult > 0);
    }
}