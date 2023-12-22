#ifndef NETUTILS_H
#define NETUTILS_H

typedef enum NetIfType {
    NetIfWlan,
    NetIfEth
} NetIfType;

typedef struct NetInterfaceInfo
{
    NetIfType type;
    int active;
    char ip[16];
    char mask[16];
    char mac[18];
} NetInterfaceInfo;


char* NetIfTypeToStr(NetIfType type);

NetInterfaceInfo GetNetInterfaceInfo(NetIfType type);

int GetMaskPrefixLenght(const char* mask);

#endif //NETUTILS_H