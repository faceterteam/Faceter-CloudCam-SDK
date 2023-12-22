#ifndef INITIALIZATIONCONFIG_H
#define INITIALIZATIONCONFIG_H

#include "WifiConfig.h"

typedef struct InitializationConfig {
    WifiConfig wifiConfig;
    char registrationToken[50];
    int prodApi;
    int micDisabled;
} InitializationConfig;


#endif