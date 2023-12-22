#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Error
{
    ErrorNo,
    ErrorConnection,
    ErrorProtocol,
    ErrorDescription,
    ErrorState
} Error;

#ifdef __cplusplus
}
#endif

#endif // ERROR_H
