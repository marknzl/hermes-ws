#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string.h>
#include "constants.h"
#include "filesyshelper.h"

#pragma comment(lib, "Ws2_32.lib")

void RunWebServer(SOCKET* listenSocket, SOCKET* clientSocket);

void InitializeWebServer()
{
    WSADATA wsaData;
    SOCKET listenSocket = INVALID_SOCKET;
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

    iResult = getaddrinfo(NULL, PORT, &hints, &result);

    if (iResult != 0)
    {
        printf("getaddrinfo() failed! Error code: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

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

    RunWebServer(&listenSocket, &clientSocket);
}

void RunWebServer(SOCKET* listenSocket, SOCKET* clientSocket)
{
    int iResult = 0;

    printf("Web Server started! Listening on port %s\n", PORT);

    while (1)
    {
        *clientSocket = accept(*listenSocket, NULL, NULL);

        if (*clientSocket == INVALID_SOCKET)
        {
            printf("accept() failed: %d\n", WSAGetLastError());
            closesocket(*listenSocket);
            WSACleanup();
            exit(1);
        }

        int bufLen = 1024;
        char recvBuf[1024];

        do
        {
            iResult = recv(*clientSocket, recvBuf, bufLen, 0);
            if (iResult > 0)
            {
                // printf("%s\n", recvBuf);

                const char* startDelimeter = "GET ";
                const char* endDelimeter = " HTTP/1.1";

                char readBuf[512];
                char str[1024];

                char* reqPath = NULL;
                char* start = NULL;
                char* end = NULL;

                start = strstr(recvBuf, startDelimeter);

                if (start != NULL)
                {
                    start += strlen(startDelimeter);
                    end = strstr(recvBuf, endDelimeter);

                    if (end != NULL)
                    {
                        reqPath = (char*) malloc((end - start) + 1);
                        memcpy(reqPath, start, (end - start));
                        reqPath[end - start] = '\0';
                        // printf("%s\n", reqPath);

                        if (PathExists(reqPath))
                        {
                            ZeroMemory(readBuf, 512);
                            ZeroMemory(str, 1024);
                            printf("Serving %s\n", reqPath);

                            FILE* f = NULL;
                            fopen_s(&f, "wwwroot/index.html", "rb");
                            if (f == NULL)
                            {
                                printf("Couldn't serve file!\n");
                                exit(1);
                            }

                            while (fgets(readBuf, sizeof(readBuf), f))
                            {
                                strcat_s(str, sizeof(readBuf), readBuf);
                            }

                            fclose(f);
                        }

                        free(reqPath);
                        reqPath = NULL;
                    }
                }

                int iSendResult = send(*clientSocket, str, (int) strlen(str), 0);

                if (iSendResult == SOCKET_ERROR)
                {
                    printf("send() failed! Error code: %d\n", WSAGetLastError());
                    closesocket(*clientSocket);
                    WSACleanup();
                    exit(1);
                }

                closesocket(*clientSocket);
            }
        } while (iResult > 0);
    }
}