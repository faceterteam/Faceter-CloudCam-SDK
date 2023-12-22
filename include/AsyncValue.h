#ifndef ASYNCVALUE_H
#define ASYNCVALUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ByteBuffer.h"

typedef struct AsyncValue AsyncValue;

AsyncValue* AsyncValueCreate();
void AsyncValueDestroy(AsyncValue* async);

void AsyncValueSetBuffer(AsyncValue* async, ByteBuffer* buffer);
ByteBuffer* AsyncValueGetBuffer(AsyncValue* async, int timeoutMSec);

void AsyncValueSetInt64(AsyncValue* async, int64_t value);
int64_t AsyncValueGetInt64(AsyncValue* async, int timeoutMSec);

void AsyncValueSetPtr(AsyncValue* async, void* ptr);
void* AsyncValueGetPtr(AsyncValue* async, int timeoutMSec);

#ifdef __cplusplus
}
#endif

#endif