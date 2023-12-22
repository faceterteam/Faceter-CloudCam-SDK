#ifndef LOGSBUFFER_H
#define LOGSBUFFER_H

#include "Mqtt.h"

void LogsBufferSetSerialNumber(const char* serialNumber);
void LogsBufferAddMessage(const char* message);
void LogsBufferStartSend(Mqtt* mqtt, const char* topic);
void LogsBufferStopSend();

#endif