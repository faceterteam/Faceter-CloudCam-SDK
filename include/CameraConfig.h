#ifndef CAMERACONFIG_H
#define CAMERACONFIG_H

typedef enum AVCodec {
    AVCodecNone,
    AVCodecH264,
    AVCodecH265,
    AVCodecAAC,
    AVCodecOPUS,
    AVCodecPCM,
    AVCodecG711U,
    AVCodecG711A
} AVCodec;

typedef struct AudioConfig {
    int micEnabled;
    AVCodec codecName;
    int sampleRate;
    int speakerEnabled;
} AudioConfig;

typedef struct VideoConfig {
    int isEnabled;
    AVCodec codecName;
    int bitrate;
    int framerate;
    int width;
    int height;
    int keyInterval;
} VideoConfig;

typedef struct ImageConfig {
    int isMirror;
    int isFlip;
} ImageConfig;

typedef struct DetectorConfig {
    int isEnabled;
    int isDebug;
} DetectorConfig;

typedef struct OsdConfig {
    int isEnabled;
    int offsetX;
    int offsetY;
    char template[50];
} OsdConfig;

typedef struct NightModeConfig {
    int isEnabled;
} NightModeConfig;

typedef struct CameraConfig {
    AudioConfig audio;
    VideoConfig mainStream;
    VideoConfig subStream;
    ImageConfig image;
    DetectorConfig detector;
    OsdConfig osd;
    NightModeConfig nightMode;
} CameraConfig;

#endif //CAMERACONFIG_H