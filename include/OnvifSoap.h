#ifndef ONVIFSOAP_H
#define ONVIFSOAP_H

#include "ClientSettings.h"
#include "NetUtils.h"
#include "HttpServer.h"

void SoapSetContext(ClientSettings *settings, const char* snapshotUrl, const char* streamUrl);
void SoapSetNetworkInterface(NetInterfaceInfo netIface);    

ServerResponse SoapDiscoveryHandler(const char* body, int bodySize);
ServerResponse SoapMediaHandler(const char* body, int bodySize);
ServerResponse SoapDeviceHandler(const char* body, int bodySize);

//WS-Discovery
char* SoapDiscoveryHello();
char* SoapDiscoveryBye();
char* SoapDiscoveryProbeMatches();
char* SoapDiscoveyResolveMatches();

//Onvif device service
char* SoapDeviceGetDeviceInformationResponse();
char* SoapDeviceGetScopesResponse();
char* SoapDeviceGetNetworkInterfacesResponse();
char* SoapDeviceGetDNSResponse();
char* SoapDeviceGetCapabilitiesResponse();
char* SoapDeviceGetServicesResponse();
char* SoapDeviceGetSystemDateAndTimeResponse();
char* SoapDeviceGetServiceCapabilitiesResponse();


//Onvif media service
char* SoapMediaGetVideoSourcesResponse();
char* SoapMediaGetProfilesResponse();
char* SoapMediaGetSnapshotUriResponse();
char* SoapMediaGetProfileResponse();
char* SoapMediaGetStreamUriResponse();
char* SoapMediaGetVideoSourceConfigurationResponse();
char* SoapMediaGetVideoEncoderConfigurationOptionsResponse();

#endif //ONVIFSOAP_H