#ifndef STORAGE_H
#define STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "InitializationConfig.h"
#include "Registration.h"

typedef struct Storage Storage;

Storage* StorageCreate(const char* configFilePath);
void StorageDestroy(Storage* storage);

InitializationConfig StorageReadInitConfig(Storage* storage);
void StorageWriteInitConfig(Storage* storage, const InitializationConfig config);

Registration StorageReadRegistration(Storage* storage);
void StorageWriteRegistration(Storage* storage, const Registration registration);

void StorageSaveChanges(Storage* storage);

#ifdef __cplusplus
}
#endif

#endif // STORAGE_H
