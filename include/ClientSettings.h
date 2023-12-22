#ifndef CLIENTSETTINGS_H
#define CLIENTSETTINGS_H

struct JsonObject;

typedef struct ClientSettings {
    char rtspMainUrl[200];
    char rtspSubUrl[200];
    char cameraModel[100];
    char firmwareSaveDir[100];
    char resourcesDir[100];
    int endpointPort;
    int discoveryPort;
    char userPwd[100];
    char certFilePath[200];
    char confFilePath[200];
    char serialNumber[50];
} ClientSettings;

int ClientSettingsParseJson(struct JsonObject* json, ClientSettings* clientSettings, const char* settingsFilePath);

#endif