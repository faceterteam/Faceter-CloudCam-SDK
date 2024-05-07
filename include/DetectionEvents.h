#ifndef DETECTIONEVENTS_H
#define DETECTIONEVENTS_H

#include <stdint.h>

typedef enum AudioEventType 
{
    AudioEventCry,
    AudioEventScream,
    AudioEventShot,
    AudioEventClap,
    AudioEventNoise
} AudioEventType;

typedef enum VideoEventType 
{
    VideoEventMotion,
    VideoEventLineCrossing,
    VideoEventIntrusion,
    VideoEventZoneEnter,
    VideoEventZoneLeave,
    VideoEventLoitering
} VideoEventType;

typedef enum ObjectType 
{
    ObjectHuman,
    ObjectVehicle,
    ObjectAnimal,
    ObjectFire,
    ObjectSmoke,
    ObjectOther,
    ObjectUnknown
} ObjectType;

typedef enum HumanGender
{
    GenderMale,
    GenderFemale
} HumanGender;

typedef enum VehicleType 
{
    VehicleCar,
    VehicleBus,
    VehicleTruck,
    VehicleBike,
    VehicleOther
} VehicleType;

typedef struct DetectionRect 
{
    int x;
    int y;
    int width;
    int height;

    struct DetectionRect* next;
} DetectionRect;

typedef struct DetectionGrid 
{
    int rowsCount;
    int colsCount;
    int cellsCount;
    uint32_t *cells;
} DetectionGrid;

typedef struct DetectionAttribute 
{
    char* key;
    char* value;

    struct DetectionAttribute* next;
} DetectionAttribute;

void PushDetectionRect(DetectionRect** top, int x, int y, int width, int height);
DetectionRect* PopDetectionRect(DetectionRect** top);

void PushDetectionAttribute(DetectionAttribute** top, const char* key, const char* value);
void PushHumanAttibutes(DetectionAttribute** top, HumanGender gender, int age);
void PushVehicleAttributes(DetectionAttribute** top, VehicleType vehicleType, const char* licensePlate);
DetectionAttribute* PopDetectionAttribute(DetectionAttribute** top);

const char* VideoEventTypeToStr(VideoEventType eventType);
const char* ObjectTypeToStr(ObjectType objectType);
const char* AudioEventTypeToStr(AudioEventType eventType);


#endif //DETECTIONEVENTS_H