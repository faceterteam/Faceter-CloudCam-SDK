#ifndef QRCODE_H
#define QRCODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "InitializationConfig.h"

InitializationConfig QRCodeParse(const char* code);

#ifdef __cplusplus
}
#endif

#endif // QRCODE_H
