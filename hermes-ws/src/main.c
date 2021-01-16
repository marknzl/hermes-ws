#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "nethelper.h"
#include "httphelper.h"
#include "file/filesyshelper.h"
#include "file/file.h"
#include "constants.h"
#include "cache/cache.h"
#include "mimehelper.h"

int main(void)
{
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
    InitializeListenSocket(&listenSocket, PORT);

    struct Cache cache;
    CacheInit(&cache, 5);
    CheckWWWRoot();

    int iResult = 0;
    printf("Web Server started! Listening on port %s\n", PORT);

    const int bufLen = 2048;
    char recvBuf[2048];

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

        struct File* file = NULL;

        do
        {
            iResult = recv(clientSocket, recvBuf, bufLen, 0);
            if (iResult > 0)
            {
                struct HTTPRequest* httpRequest = ParseHTTPRequest(recvBuf);
                char* filePath = GetFullFilePath(httpRequest->resourcePath);

                if (!strcmp(filePath, "wwwroot/"))
                    filePath = "wwwroot/index.html";

                file = CacheGet(&cache, filePath);
                if (CacheGet(&cache, filePath) == NULL)
                {
                    file = FileLoad(filePath);
                    if (file != NULL)
                    {
                        printf("Loaded file '%s'\n", filePath);
                    }
                    else
                    {
                        file = FileLoad("wwwroot/404.html");
                    }
                    CacheInsert(&cache, filePath, file, FileDestroy);
                }

                struct HTTPResponse* httpResponseHeader = CreateHTTPResponseHeader(httpRequest, file->fileSize, GetMimeType(file->fileName));
                send(clientSocket, httpResponseHeader->response, strlen(httpResponseHeader->response), 0);
                send(clientSocket, file->fileContent, file->fileSize, 0);

                DestroyHTTPRequest(httpRequest);
                DestroyHTTPResponse(httpResponseHeader);
            }
        } while (iResult > 0);
    }
}