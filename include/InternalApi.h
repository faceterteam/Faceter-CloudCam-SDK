#ifndef INTERNALAPI_H
#define INTERNALAPI_H

#include "ClientSettings.h"
#include "MotionEventSender.h"
#include "NetUtils.h"

typedef void (*NetworkInterfaceChangeHandler)(NetInterfaceInfo netIface);

void InternalApiStart(ClientSettings *clientSettings, GetSnapshotFunction getSnapshot, NetworkInterfaceChangeHandler networkChange);
void InternalApiStop();

#endif