#ifndef MOTIONEVENTSENDER_H
#define MOTIONEVENTSENDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Mqtt.h"
#include "ByteBuffer.h"

typedef struct MotionEventSender MotionEventSender;

typedef ByteBuffer* (*GetSnapshotFunction)(int timeoutMSec);

MotionEventSender* MotionEventSenderCreate(Mqtt* mqtt, const char* deviceId, GetSnapshotFunction getSnapshot);
void MotionEventSenderDestroy(MotionEventSender* sender);

void MotionEventSenderOnMotion(MotionEventSender* sender);

const char* MotionEventSenderTopicName(MotionEventSender* sender);

#ifdef __cplusplus
}
#endif

#endif