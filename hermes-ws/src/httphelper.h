#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

struct HTTPRequest
{
	char* resourcePath;
};

struct HTTPResponse
{
	char* response;
};

void HTTPRequestInit(struct HTTPRequest* httpRequest);
void HTTPResponseInit(struct HTTPResponse* httpResponse);

struct HTTPRequest* ParseHTTPRequest(char* recvBuf);
void DestroyHTTPRequest(struct HTTPRequest* httpRequest);
struct HTTPResponse* CreateHTTPResponseHeader(struct HTTPRequest* httpRequest, long fileSize, char* mimetype);
void DestroyHTTPResponse(struct HTTPResponse* httpResponse);

#endif