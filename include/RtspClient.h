#ifndef RTSPCLIENT_H
#define RTSPCLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "State.h"
#include "ByteBuffer.h"
#include "Types.h"

typedef struct RtspClient RtspClient;
typedef struct TransferBuffer TransferBuffer;

typedef struct RtpStream {
    char isVideo;
    char channel;
    char payloadType;
    char control[100];
} RtpStream;

RtspClient* RtspClientCreate(const char* rtspServerUrl);
void RtspClientDestroy(RtspClient* client);

Boolean RtspClientOptions(RtspClient* client);
Boolean RtspClientDescribe(RtspClient* client, ByteBuffer *description);
Boolean RtspClientAnnounce(RtspClient* client, ByteBuffer *description);
Boolean RtspClientSetup(RtspClient* client, RtpStream streams[], size_t streamsCount, Boolean recordMode);
Boolean RtspClientPlay(RtspClient* client, TransferBuffer* buffer);
Boolean RtspClientRecord(RtspClient* client, TransferBuffer* buffer);
Boolean RtspClientTeardown(RtspClient* client);

State RtspClientGetState(RtspClient* client);

Boolean RtspClientSameUrl(RtspClient* client, const char* rtspServerUrl);

#ifdef __cplusplus
}
#endif

#endif