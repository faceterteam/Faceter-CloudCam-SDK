#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "NetUtils.h"

typedef enum ServerStatusCode {
    ServerStatusOk,
    ServerStatusError,
    ServerStatusNotFound
} ServerStatusCode;

typedef struct ServerResponse {
    ServerStatusCode statusCode;
    char* headers;
    char* body;
    long int bodySize;
    int chunked;
} ServerResponse;

typedef ServerResponse (* ServerRequestHandler)(const char* body, int bodySize);

void HttpServerStart(int httpPort);
void HttpServerStop();

void HttpServerAddRequestHandler(const char* method, const char* url, ServerRequestHandler handler);
void HttpServerRemoveRequestHandler(const char* method, const char* url);

#endif //HTTPSERVER_H