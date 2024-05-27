#ifndef FACETERCLIENT_H
#define FACETERCLIENT_H

#include "ClientSettings.h"
#include "ControlFunction.h"
#include "WifiConfig.h"
#include "DetectionEvents.h"

/*
 * Library initialization function, must be called first. 
 * Function validates and parses settings from file
 *
 * @param controlFunction callback for controlling app from library
 * @param  settings library settings
 * 
 * @return 0 if init ok, -1 on error
 */
int FaceterClientInit(ControlFunction controlFunction, ClientSettings settings);

//start-stop main client logic

/*
 * Start inner processing loop. 
 * After start controlFunction will be called with differrent codes
 */
void FaceterClientStart();
/*
 * Stop inner processing loop
 */
void FaceterClientStop();

/*
 * Send detected video event to the library
 *
 * @param eventTimestampSec UTC linux timestamp when event occured
 * 
 * @param eventType type of detected video event.
 * If does not matched anything else in the VideoEventType use VideoEventMotion
 * 
 * @param objectType type of detected object.
 * If does not matched anything else in the ObjectType use ObjectOther
 * 
 * @param attributesList list of object's attributes (key - value pairs).
 * Could be NULL. Use PushDetectionAttribute for adding attribute, 
 * PushHumanAttibutes for human attributes,  
 * PushVehicleAttributes for vehicle attributes
 * 
 * @param detectionGrid desribes grid - number of rows, cols and cells, where
 * event was detected
 * 
 * @param snapshotImage image of the detected event. Could be NULL
 * @param snapshotBytesCount size of snaphot, 0 if snapshotImage is NULL
 * 
 */
void FaceterClientOnVideoEvent(int64_t eventTimestampSec, VideoEventType eventType, ObjectType objectType, 
    DetectionAttribute *attributesList, DetectionGrid detectionGrid, 
    char* snapshotImage, long int snapshotBytesCount);


/*
 * Method for start sending long detections
 *
 * params descripion in FaceterClientOnVideoEvent
 */
void FaceterClientOnVideoEventStart(int64_t eventTimestampSec, VideoEventType eventType, ObjectType objectType, 
    DetectionAttribute *attributesList, DetectionGrid detectionGrid, 
    char* snapshotImage, long int snapshotBytesCount);
/*
 * Method for update deetction grid for long detection
 *
 * params descripion in FaceterClientOnVideoEvent
 */
void FaceterClientOnVideoEventUpdate(int64_t eventTimestampSec, VideoEventType eventType, ObjectType objectType, 
    DetectionGrid detectionGrid);    

/*
 * Method for end sending long detections
 *
 * params descripion in FaceterClientOnVideoEvent
 */
void FaceterClientOnVideoEventEnd(int64_t eventTimestampSec, VideoEventType eventType, ObjectType objectType, 
    DetectionGrid detectionGrid);

/*
 * Send detected audio event to the library
 *
 * @param eventType type of detected audio event.
 * If does not matched anything else in the AudioEventType use AudioEventNoise
 */
void FaceterClientOnAudioEvent(AudioEventType eventType);

/*
 * Callback to the library with camera snapshot
 * 
 * @param imageData snapshot jpeg bytes
 * @param imageBytesCount size in bytes
 */
void FaceterClientOnSnapshot(const char* imageData, long int imageBytesCount);
/*
 * Callback to the library with qr code data
 * 
 * @param qr parsed code string from scanner
 */
void FaceterClientOnQrScanned(const char* qr);

/*
 * Reset client state to default
 */
void FaceterClientReset();

#endif //FACETERCLIENT_H
