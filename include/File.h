#ifndef FILE_H
#define FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ByteBuffer.h"

size_t FileWrite(const char* fileName, ByteBuffer* buffer);
ByteBuffer* FileRead(const char* fileName);
int FileExist(const char* fileName);

#ifdef __cplusplus
}
#endif

#endif