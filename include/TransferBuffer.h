#ifndef TRANSFERBUFFER_H
#define TRANSFERBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "RtspClient.h"

#define TransferBufferPacketSize 1500

typedef struct Packet {
    char data[TransferBufferPacketSize];
    size_t dataSize;
} Packet;

typedef struct TransferBuffer TransferBuffer;

TransferBuffer* TransferBufferCreate(void);
void TransferBufferDestroy(TransferBuffer* buffer);

void TransferBufferAddPacket(TransferBuffer* buffer, const char* packet, size_t packetSize);
Packet TransferBufferWaitPacket(TransferBuffer* buffer);
size_t TransferBufferGetSize(TransferBuffer* buffer);
void TransferBufferOpen(TransferBuffer* buffer, RtpStream streams[], size_t streamCount);
void TransferBufferClose(TransferBuffer* buffer);


#ifdef __cplusplus
}
#endif

#endif