#ifndef CLIENTSETTINGS_H
#define CLIENTSETTINGS_H

typedef struct ClientSettings {
    char rtspMainUrl[200];
    char rtspSubUrl[200];
    char cameraModel[100];
    char cameraVendor[100];
    char firmwareVersion[50];
    char appVersion[50];
    char hardwareId[100];
    char serialNumber[50];
    int endpointPort;
    int discoveryPort;
    char rtspCredentials[100];
    char certFilePath[200];
    char confFilePath[200];
    char featuresFilePath[200];
} ClientSettings;

#endif