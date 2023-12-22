#ifndef MQTT_H
#define MQTT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ByteBuffer.h"
#include "Types.h"
#include <stdbool.h>

typedef struct MqttMessage {
    char topic[100];
    bool retain;
    ByteBuffer* body;
} MqttMessage;

typedef struct MqttAuth {
    char host[100];
    int port;
    char user[50];
    char pass[50];
    char clientId[50];
    char qos;
} MqttAuth;

typedef void (*SubscriptionHandler)(const char* topic, ByteBuffer* buffer, void *subscriber);

typedef struct Mqtt Mqtt;

Mqtt* MqttCreate(size_t messageBufferSize, SubscriptionHandler handler, void* subscriber);
void MqttDestroy(Mqtt* mqtt);

bool MqttConnect(Mqtt* mqtt, MqttAuth auth, MqttMessage willMessage, int timeoutSec);

void MqttDisconnect(Mqtt* mqtt);

bool MqttIsConnected(Mqtt* mqtt);

bool MqttSubscribe(Mqtt* mqtt, const char* topic);

bool MqttPublish(Mqtt* mqtt, MqttMessage message);

size_t MqttMessageBufferSpace(Mqtt* mqtt);

#ifdef __cplusplus
}
#endif

#endif //MQTT_H