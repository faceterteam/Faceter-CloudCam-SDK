#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

#define SETTINGS_NIGHTMODE_ON "on"
#define SETTINGS_NIGHTMODE_OFF "off"
#define SETTINGS_NIGHTMODE_AUTO "auto"

#define SETTINGS_LINE_DIRECTION_AB "ab"
#define SETTINGS_LINE_DIRECTION_BA "ba"
#define SETTINGS_LINE_DIRECTION_BOTH "both"

#define SETTINGS_ROTATE_DIRECTION_HORIZONTAL "horizontal"
#define SETTINGS_ROTATE_DIRECTION_VERTICAL "vertical"
#define SETTINGS_ROTATE_DIRECTION_BOTH "both"

#define SETTINGS_SCENE_OUTDOOR "outdoor"
#define SETTINGS_SCENE_INDOOR "indoor"

typedef struct SettingMicrophone {
    bool enable;
    int sensitivity;
} SettingMicrophone;

typedef struct SettingNightmode {
    char mode[10]; //on|off|auto
} SettingNightmode;

typedef struct SettingTimezone {
    char value[50];
} SettingTimezone;

typedef struct SettingImageRotation {
    bool enable;
} SettingImageRotation;

typedef struct SettingImageMirror {
    bool horizontal;
    bool vertical;
} SettingImageMirror;

typedef struct SettingPtz {
    bool horizontal;
    bool vertical;
    bool zoom;
} SettingPtz;

typedef struct SettingAreaMatrixCell {
    char value[10];//00fb2c3e\0
} SettingAreaMatrixCell;

typedef struct SettingAreaRect {
    int x;
    int y;
    int w;
    int h;
} SettingAreaRect;

typedef struct SettingAreaPolygonPoint {
    int x;
    int y;
} SettingAreaPolygonPoint;

typedef struct SettingAreaPolygon {
    int pointsCount;
    SettingAreaPolygonPoint *points;
} SettingAreaPolygon;

typedef struct SettingLine {
    int ax;
    int ay;
    int bx;
    int by;
    char direction[10]; //ab | ba | both
} SettingLine;

typedef struct SettingArea {
    int matrixCellsCount;
    SettingAreaMatrixCell *matrix;
    int rectsCount;
    SettingAreaRect* rects;
    int poligonsCount;
    SettingAreaPolygon* poligons;
} SettingArea;

typedef struct SettingMotionDetection {
    bool enable;
    int sensitivity;
    int threshold;
    SettingArea area;
} SettingMotionDetection;

typedef struct SettingHumanDetection {
    bool enable;
    char scene[10]; //indoor | outdoor
    int confidence;
    SettingArea area;
} SettingHumanDetection;

typedef struct SettingVehicleDetection {
    bool enable;
    char scene[10]; //indoor | outdoor
    int confidence;
    SettingArea area;
} SettingVehicleDetection;

typedef struct SettingLineCrossingDetection {
    bool enable;
    int sensitivity;
    int linesCount;
    SettingLine *lines;
} SettingLineCrossingDetection;

typedef struct SettingIntrusionDetection {
    bool enable;
    int sensitivity;
    char scene[10]; //indoor | outdoor
    SettingArea area;
} SettingIntrusionDetection;

typedef struct SettingNetworkIPv4 {
    bool dhcp; //static | dhcp
    char ip[20];
    char mask[20];
    char gateway[20];
    char primaryDns[20];
    char secondaryDns[20];
} SettingNetworkIPv4;

typedef struct SettingNetworkIPv6 {
    bool dhcp; //static | dhcp
    char ip[50];
    int prefixLen;
    char gateway[50];
    char primaryDns[50];
    char secondaryDns[50];
} SettingNetworkIPv6;

typedef struct SettingNetworkWifi {
    char essid[50];
    char signal[10];
} SettingNetworkWifi;

typedef struct SettingNetworkInterface {
    char name[20];
    char mac[20];
    bool local;//local | wifi
    bool active;
    SettingNetworkIPv4 ipv4;
    SettingNetworkIPv6 ipv6;
    SettingNetworkWifi wifi;

} SettingNetworkInterface;

typedef struct SettingNetwork {
    int interfacesCount;
    SettingNetworkInterface* interfaces;
} SettingNetwork;

typedef struct SettingEventDetection {
    SettingMotionDetection motion;
    SettingHumanDetection human;
    SettingVehicleDetection vehicle;
    SettingLineCrossingDetection lineCrossing;
    SettingIntrusionDetection intrusion;
} SettingEventDetection;

typedef struct Settings {
    SettingMicrophone microphone;
    SettingNightmode nightmode;
    SettingTimezone timezone;
    SettingImageRotation imageRotation;
    SettingImageMirror imageMirror;
    SettingPtz ptz;
    SettingEventDetection eventDetection;
    SettingNetwork network;
} Settings;

typedef struct JsonObject JsonObject;

char* SettingMicrophoneToJson(SettingMicrophone setting, JsonObject* featuresObject);
SettingMicrophone SettingMicrophoneFromJson(const char* json);

char* SettingNightmodeToJson(SettingNightmode setting, JsonObject* featuresObject);
SettingNightmode SettingNightmodeFromJson(const char* json);

char* SettingTimezoneToJson(SettingTimezone setting, JsonObject* featuresObject);
SettingTimezone SettingTimezoneFromJson(const char* json);

char* SettingImageRotationToJson(SettingImageRotation setting, JsonObject* featuresObject);
SettingImageRotation SettingImageRotationFromJson(const char* json);

char* SettingImageMirrorToJson(SettingImageMirror setting, JsonObject* featuresObject);
SettingImageMirror SettingImageMirrorFromJson(const char* json);

char* SettingPtzToJson(SettingPtz setting, JsonObject* featuresObject);
SettingPtz SettingPtzFromJson(const char* json);

char* SettingAreaRectToJson(SettingAreaRect rect);
SettingAreaRect SettingAreaRectFromJson(const char* json);

char* SettingLineToJson(SettingLine line);
SettingLine SettingLineFromJson(const char* json);

void SettingAreaToJson(SettingArea setting, JsonObject* parentObject);
SettingArea SettingAreaFromJson(JsonObject* parentObject);

char* SettingMotionDetectionToJson(SettingMotionDetection setting, JsonObject* featuresObject);
SettingMotionDetection SettingMotionDetectionFromJson(const char* json);

char* SettingHumanDetectionToJson(SettingHumanDetection setting, JsonObject* featuresObject);
SettingHumanDetection SettingHumanDetectionFromJson(const char* json);

char* SettingVehicleDetectionToJson(SettingVehicleDetection setting, JsonObject* featuresObject);
SettingVehicleDetection SettingVehicleDetectionFromJson(const char* json);

char* SettingLineCrossingDetectionToJson(SettingLineCrossingDetection setting, JsonObject* featuresObject);
SettingLineCrossingDetection SettingLineCrossingDetectionFromJson(const char* json);

char* SettingIntrusionDetectionToJson(SettingIntrusionDetection setting, JsonObject* featuresObject);
SettingIntrusionDetection SettingIntrusionDetectionFromJson(const char* json);

char* SettingNetworkInterfaceToJson(SettingNetworkInterface interface);
SettingNetworkInterface SettingNetworkInterfaceFromJson(const char* json);

char* SettingNetworkIPv4ToJson(SettingNetworkIPv4 ipAddr);
SettingNetworkIPv4 SettingNetworkIPv4FromJson(const char* json);

char* SettingNetworkIPv6ToJson(SettingNetworkIPv6 ipAddr);
SettingNetworkIPv6 SettingNetworkIPv6FromJson(const char* json);

char* SettingNetworkWifiToJson(SettingNetworkWifi wifi);
SettingNetworkWifi SettingNetworkWifiFromJson(const char* json);

char* SettingNetworkToJson(SettingNetwork network);
SettingNetwork SettingNetworkFromJson(const char* json);

char* SettingEventDetectionToJson(SettingEventDetection setting, JsonObject* featuresObject);
SettingEventDetection SettingEventDetectionFromJson(const char* json);

char* SettingsToJson(Settings settings, JsonObject* featuresObject);
Settings SettingsFromJson(const char* json);


#endif //SETTINGS_H