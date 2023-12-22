#ifndef SERIALNUMBER_H
#define SERIALNUMBER_H

typedef enum SerialNumberType {
    SNFaceter,
    SNMac,
    SNTime,
    SNUnknown,
    SNEmpty
} SerialNumberType;

void SerialNumberFromMacAddress(char* output, int outputSize, const char* macAddress);
void SerialNumberFromTimestamp(char* output, int outputSize);
SerialNumberType SerialNumerGetType(const char* serialNumber);

#endif