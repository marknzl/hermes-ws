#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "httphelper.h"
#include "constants.h"
#include "file/file.h"
#include "mimehelper.h"

void HTTPRequestInit(struct HTTPRequest* httpRequest)
{
	httpRequest->resourcePath = NULL;
}

void DestroyHTTPRequest(struct HTTPRequest* httpRequest)
{
	free(httpRequest->resourcePath);
	free(httpRequest);
}

struct HTTPRequest* ParseHTTPRequest(char* recvBuf)
{
	struct HTTPRequest* httpRequest = malloc(sizeof(struct HTTPRequest));
	if (httpRequest == NULL)
		return NULL;
	HTTPRequestInit(httpRequest);

	//const char* str = "GET /index.html HTTP/1.1";

	const char* startPathCharPos = strchr(recvBuf, ' ') + 1;
	const char* endPathCharPos = strchr(startPathCharPos, ' ');
	long diff = endPathCharPos - startPathCharPos;

	char* buf = malloc((sizeof(char) * diff) + 1);
	if (buf == NULL)
		return NULL;
	memcpy(buf, startPathCharPos, (size_t)diff + 1);
	buf[diff] = '\0';

	httpRequest->resourcePath = buf;
	//printf("Request for resource: %s\n", httpRequest->resourcePath);
	return httpRequest;
}

void HTTPResponseInit(struct HTTPResponse* httpResponse)
{
	//httpResponse->responseLength = 0;
	httpResponse->response = NULL;
}

void DestroyHTTPResponse(struct HTTPResponse* httpResponse)
{
	free(httpResponse->response);
	free(httpResponse);
}

struct HTTPResponse* CreateHTTPResponseHeader(struct HTTPRequest* httpRequest, long fileSize, char* mimetype)
{
	struct HTTPResponse* httpResponse = malloc(sizeof(struct HTTPResponse));
	if (httpResponse == NULL)
		return NULL;
	HTTPResponseInit(httpResponse);
	char* buf = malloc(sizeof(char) * 2048);
	if (buf == NULL)
		return NULL;

	char pathBuf[2048];
	if (!strcmp(httpRequest->resourcePath, "/"))
	{
		sprintf_s(pathBuf, 2048, "%s%s", ROOT_DIR, "index.html");
	}
	else
	{
		sprintf_s(pathBuf, 2048, "%s%s", ROOT_DIR, httpRequest->resourcePath + 1);
	}

	sprintf_s(buf, 2048, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\ncharset=utf-8\r\nContent-Length: %d\r\nX-Powered-By: hermes-ws\r\n\r\n", mimetype, fileSize - 1);

	httpResponse->response = buf;

	return httpResponse;
}