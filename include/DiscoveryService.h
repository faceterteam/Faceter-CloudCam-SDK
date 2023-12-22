#ifndef DISCOVERYSERVICE_H
#define DISCOVERYSERVICE_H

#include "FaceterClient.h"
#include "NetUtils.h"
#include "HttpServer.h"

void DiscoveryServiceStart(int discoveryPort);
void DiscoveryServiceStop();
int DiscoveryServiceAddMulticastGroup(const char* ip);

#endif //DISCOVERYSERVICE_H