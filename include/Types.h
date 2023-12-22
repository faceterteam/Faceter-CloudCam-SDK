#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Boolean {
    BOOL_FALSE,
    BOOL_TRUE
} Boolean;

typedef struct RtspRoute {
    char inputUrl[200];
    char outputUrl[200];
} RtspRoute;

typedef struct StreamingConfig {
    RtspRoute mainStreamRoute;
    RtspRoute subStreamRoute;
} StreamingConfig;

#ifdef __cplusplus
}
#endif

#endif // TYPES_H