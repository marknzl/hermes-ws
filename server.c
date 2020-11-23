#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "constants.h"
#include "nethelper.h"
#include "filesyshelper.h"
#include "mimehelper.h"

#pragma comment(lib, "Ws2_32.lib")

void HandleHTTPRequest(char* req, char** resourcePath)
{
    const char* startDelimeter = "GET ";
    const char* endDelimeter = " HTTP/1.1";

    char readBuf[512];
    char str[1024];

    char* start = NULL;
    char* end = NULL;

    start = strstr(req, startDelimeter);

    if (start != NULL)
    {
        start += strlen(startDelimeter);
        end = strstr(req, endDelimeter);

        if (end != NULL)
        {
            *resourcePath = (char*) malloc((end - start) + 1);
            memcpy(*resourcePath, start, (end - start));
            char* reqStr = *resourcePath;
            reqStr[end - start] = '\0';
        }
    }
}

void GetResource(char* resourcePath, char* content)
{
    char readBuf[512];
    char fullPath[512] = "wwwroot";

    if (!strcmp(resourcePath, "/"))
    {
        strcpy(resourcePath, "/index.html");
    }

    if (PathExists(resourcePath))
    {
        printf("GET %s\n", resourcePath);
        ZeroMemory(readBuf, 512);
        ZeroMemory(content, 1024);

        FILE* f = NULL;
        strcat(fullPath, resourcePath);
        fopen_s(&f, fullPath, "rb");

        if (f == NULL)
        {
            printf("Couldn't serve file!\n");
            return;
            exit(1);
        }

        while (fgets(readBuf, sizeof(readBuf), f))
        {
            strcat_s(content, sizeof(readBuf), readBuf);
        }

        fclose(f);
    }
}

void BuildResponse(char* response, char* mimeType, char* content)
{
    strcat(response, "HTTP/1.1 200 OK\r\nContent-Type: ");
    strcat(response, mimeType);
    strcat(response, "\ncharset=utf-8\r\n\r\n");
    strcat(response, content);
}

void SendResponse(SOCKET* clientSocket, char* content, int length)
{
    int iSendResult = send(*clientSocket, content, length, 0);

    if (iSendResult == SOCKET_ERROR)
    {
        printf("send() failed! Error code: %d\n", WSAGetLastError());
        closesocket(*clientSocket);
        WSACleanup();
        exit(1);
    }

    closesocket(*clientSocket);
}

int main(void)
{
    CheckWWWRoot();

    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
    InitializeListenSocket(&listenSocket, PORT);

    int iResult = 0;

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
        char recvBuf[1024]; // client request
        char content[1024]; // file content
        char response[1024] = "";   // server response

        do
        {
            iResult = recv(clientSocket, recvBuf, bufLen, 0);
            if (iResult > 0)
            {
                // printf("%s\n", recvBuf);
                char* resourcePath = NULL;
                HandleHTTPRequest(recvBuf, &resourcePath);

                GetResource(resourcePath, content);
                BuildResponse(response, GetMimeType(resourcePath), content);
                SendResponse(&clientSocket, response, (int) strlen(response));

                memset(content, 0, sizeof(content));
                memset(response, 0, sizeof(response));
                free(resourcePath);
                resourcePath = NULL;
            }
        } while (iResult > 0);
    }

    return 0;
}