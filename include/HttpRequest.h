#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ByteBuffer.h"

typedef enum HttpRequestType {
    HttpRequestTypePOST,
    HttpRequestTypeGET
} HttpRequestType;

typedef enum HttpResponseCode {
    HttpResponseCodeOk,
    HttpResponseCodeConnectionError,
    HttpResponseCodeBadRequest
} HttpResponseCode;

typedef struct HttpResponse {
    HttpResponseCode code;
    int httpStatusCode;
    ByteBuffer *body;
} HttpResponse;

typedef struct HttpRequest HttpRequest;

HttpRequest* HttpRequestCreate(HttpRequestType type, const char* url, const char* certFilePath);
void HttpRequestDestroy(HttpRequest* request);

void HttpRequestSetUserPassword(HttpRequest* request, const char* userPwd);

void HttpRequestAddHeader(HttpRequest* request, const char* title, const char* value);

void HttpRequestAddStringBody(HttpRequest* request, const char* dataFields);

void HttpRequestAddByteBufferBody(HttpRequest* request, ByteBuffer *buffer);

HttpResponse HttpRequestExec(HttpRequest* request);

HttpResponseCode HttpRequestDownload(HttpRequest* request, const char* saveFileName);

#ifdef __cplusplus
}
#endif

#endif
