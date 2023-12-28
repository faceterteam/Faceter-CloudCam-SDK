#ifndef FACETERCLIENT_H
#define FACETERCLIENT_H

#include "CameraConfig.h"
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
 * Set status code of control operation.
 * Should be called as response to controlFunction
 * 
 * @param controlCode operation control code from controlFunction
 * @param statusCode operation result
 */
void FaceterClientSetControlStatus(ClientControlCode controlCode, ClientStatusCode statusCode);

/*
 * Send detected video event to the library
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
 * @param relativeBoundingRectList list of detected objects bounding rects in relative coordinates.
 * Could be NULL. Use PushDetectionRect for adding next DetectionRect. 
 * 0 <= x, y, width, height < 100
 * 
 * @param snapshotImage image of the detected event. Could be NULL
 * @param snapshotSize size of snaphot, 0 if snapshotImage is NULL
 * 
 */
void FaceterClientOnVideoEvent(VideoEventType eventType, ObjectType objectType, 
    DetectionAttribute *attributesList, DetectionRect *relativeBoundingRectList, 
    char* snapshotImage, long int snapshotSize);


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
 * @param imageSize bytes count
 */
void FaceterClientOnSnapshot(const char* imageData, long int imageSize);
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
