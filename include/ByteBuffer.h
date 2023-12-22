#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct ByteBuffer ByteBuffer;

ByteBuffer* ByteBufferCreate(size_t size);
ByteBuffer* ByteBufferCreateWithData(const char* data, size_t size);
void ByteBufferDestroy(ByteBuffer* buffer);

void ByteBufferClear(ByteBuffer* buffer);

size_t ByteBufferAppendBytes(ByteBuffer* buffer, const char* bytes, size_t bytesCount);

size_t ByteBufferReadBytes(ByteBuffer* buffer, char* destBytes, size_t destBytesCount);

char* ByteBufferGetDataPointer(ByteBuffer* buffer);
size_t ByteBufferGetDataSize(ByteBuffer* buffer);

void ByteBufferRemoveOwnership(ByteBuffer* buffer);

#ifdef __cplusplus
}
#endif

#endif