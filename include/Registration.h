#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "ControlFunction.h"
#include "JsonObject.h"

typedef struct Registration {
    char deviceId[50];
    char deviceToken[50];
} Registration;

typedef struct Storage Storage;

typedef Registration (*RegistrationRequest)(const char *registrationToken, const char *macAddress, int useProductionApi);

int RegistrationCheckQr(const char* qrCode);
int RegistrationCheckJson(JsonObject* json);
int RegistrationCompleted();

Registration RegistrationMake(Storage* storage, ControlFunction controlFunction, RegistrationRequest requestFunction);
void RegistrationReset(Storage* storage);

#endif