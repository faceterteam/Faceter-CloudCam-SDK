#ifndef FACETERCLIENT_H
#define FACETERCLIENT_H

#include "CameraConfig.h"
#include "ClientSettings.h"
#include "ControlFunction.h"
#include "WifiConfig.h"

/*
 * Library initialization function, must be called first. 
 * Function validates and parses settings from file
 *
 * @param controlFunction callback for controlling app from library
 * @param settingsFilePath path to the json settings file
 * @param serialNumber device unique serial number, could be @c NULL
 * 
 * @return 0 if init ok, -1 on error
 */
int FaceterClientInit(ControlFunction controlFunction, const char* settingsFilePath, const char* serialNumber);

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
 * Camera configuration parsed from json settings file. 
 * Could be used to setup camera parameters
 * @return CameraConfig structure pointer
 */
CameraConfig* FaceterClientGetCameraConfig();
/*
 * Custom configuration string from settings file
 * @return raw string from "customConfig" section
 */
const char* FaceterClientGetCustomConfigStr();
/*
 * Client settings parsed from settings json file
 * @return ClientSettings structure pointer
 */
ClientSettings* FaceterClientGetClientSettings();

/*
 * Set status code of control operation.
 * Should be called as response to controlFunction
 * 
 * @param controlCode operation control code from controlFunction
 * @param statusCode operation result
 */
void FaceterClientSetControlStatus(ClientControlCode controlCode, ClientStatusCode statusCode);

/*
 * Callback to the library when motion detected
 */
void FaceterClientOnMotion();

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
