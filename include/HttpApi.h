#ifndef HTTPAPI_H
#define HTTPAPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Types.h"
#include "HttpRequest.h"
#include "Registration.h"

Registration HttpApiRegisterCamera(const char* baseUrl, const char* certFilePath, const char* registrationToken,
    const char* macAddress, const char* cameraModel);

StreamingConfig HttpApiStartStreaming(
    const char* baseUrl, const char* certFilePath, const Registration registration,
    const char* version, const char* cameraModel,
    HttpResponseCode* responseCode, int* httpStatusCode);

#ifdef __cplusplus
}
#endif

#endif