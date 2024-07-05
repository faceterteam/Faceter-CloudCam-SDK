# Faceter CloudCam SDK

## Overview

This SDK is designed to integrate IP cameras with the Faceter cloud video surveillance service. The library allows you to implement plug&play connection of cameras to the cloud and mobile applications, management of basic settings, transfer of AI detections and some service functions necessary for customer support

![Integration scheme](integration_scheme.png)

**Device App** is an application on the camera that implements the connection between the Faceter CloudCam SDK and the camera platform

**Platform SDK** is a camera manufacturer’s platform, it can be the original SDK from the chip manufacturer (SigmaStar, Ingenic, Goke, etc.), or the vendor’s middleware platform built on top of them

**Faceter CloudCam SDK** – modules that implement interaction with the cloud and Faceter applications. The SDK requires a local RTSP stream, which is transmitted to the Faceter cloud using RTSP-push technology

## Intergration sample

An example of code with stubs describing intergation process is shown in [integration_sample.c](integration_sample.c) 

## Step-by-Step Guide

1. Initialize library with FaceterClientInit 
2. Implement registration in Faceter cloud through QR code scanning and WS-Discovery
3. Implement additional methods for controlling the camera via the cloud: 
   turning the microphone on/off, rotating the image, ...
4. Implement transmission of all detections supported by the camera
5. Implement service functions:
   * Jpeg snapshot
   * Registration reset
   * Serial number
   * LED indication
   * OTA firmware update

### Initialization

First step of integration process is library initialization

```
const char* serialNumber = GetSerialNumber();

ClientSettings settings = {
    .cameraModel = "MyModel",
    .cameraVendor = "Vision",
    .appVersion = "1.0.0",
    .firmwareVersion = "Camera_1.0.3",
    .hardwareId = "t31_gc2053",
    .certFilePath = "/etc/ssl/certs/ca-certificates.crt",
    .confFilePath = "/etc/faceter-camera.conf",
    .rtspMainUrl = "rtsp://127.0.0.1/stream=0",
    .rtspCredentials = "root:12345",
    .featuresFilePath = "/etc/camera_features.json"
};
strcpy(settings.serialNumber, serialNumber);

//library initialization
if (FaceterClientInit(ControlHandler, settings) < 0) {
    return 0;
}
```

FaceterClientInit accepts 2 parameters:

- **ControlFunction** - callback from library with control code and parameters
- **settings** - ClientSettings for library initalization

Returns 0 on success and -1 if some error value occurs

#### ClientSettings description
* cameraModel - camera model name ("MyModel")
* cameraVendor - device manufacturer ("Vision")
* serialNumber - unique stable camera serial number
* appVersion - version of the applcation using library ("1.0.0")
* firmwareVersion - camera firmware version ("Camera_1.0.3")
* hardwareId - information about hardware, such as processor and sensor ("t31_gc2053")
* certFilePath - path to the SSL certificate file, used in HTTPS connections ("/etc/ssl/certs/ca-certificates.crt")
* confFilePath - path to the file in the writable location where library will store it's state ("/etc/faceter-camera.conf")
* rtspMainUrl - main stream RTSP url without credentials ("rtsp://127.0.0.1/stream=0")
Library currently supports only video codec H264 and audio codec AAC
* rtspSubUrl - second RTSP stream, could be empty
* rtspCredentials - user name and password for acessing RTSP stream ("root:12345")
* featuresFilePath - path to the json file with all camera features decription

### Registration

WiFi cameras require QR scanner. Also connection to WiFi network with ssid and password should be implemented. Registration process in this case consists of 3 steps:
1. Library sends operation code **ControlCodeStartScanQr** to start QR scanner
   
   ```
   case ControlCodeStartScanQr: {
      //start QR code scanning
      QrScannerStart();
      break;
   }
   ```
   
2. Show QR code in Faceter application to the camera
3. When QR code is scanned with QR scanner - call `FaceterClientOnQrScanned` with qr code value string

   ```
   void OnQrScannerScanSuccess(char* qrCode) 
   {
       //pass scanned string to Faceter library
       FaceterClientOnQrScanned(qrCode);
   }
   ```
   
4. If QR code is correct, code **ControlCodeStopScanQr** will be sent to stop QR scanner

   ```
   case ControlCodeStopScanQr: {
       //stop QR code scanning
       QrScannerStop();
       break;
   }
   ```
5. Library will sent **ControlCodeSetupWifi** with param _WifiConfig*_ to setup WiFi network

   ```
   ...
   case ControlCodeSetupWifi: {
        //setup wifi with ssid and password
        WifiConfig* config = (WifiConfig*)param;
        WifiSetup(config->network, config->password);
        break;
    }
   ...
   
   /*
    * Implement wifi setup for wireless cameras
    */
   void WifiSetup(const char* networkSsid, const char* password) 
   {
   }
   ```
   
6. If camera obtains internet access, registration will be finished. Otherwise process will be repeted from step 1

### Controlling camera

Faceter application can send commands to control camera parameters, such as wifi params.
Library will send control code ControlCodeSetupWiifi
```
//control handler fragment
...
case ControlCodeSetupWifi: {
    WifiConfig* config = (WifiConfig*)param;
    WifiSetup(config->network, config->password);
    break;
}
```
After operation completes application must return  **StatusCodeOk** if operation succeed or other on fail.
If operation not supported statusCode **MUST** be set to **StatusCodeNotSupported**.
if operation not implemented yet use **StatusCodeNotImplemented**.
For example if camera not supports audio playback, it will return status without processing operation
```
case ControlCodePlayAudio: {
  //play audio PCM buffer
  BufferParam* audioBuffer = (BufferParam*)param;
  statusCode = StatusCodeNotSupported;
  break;
}
```
If camera can play audio - status code will be **StatusCodeOk**
```
case ControlCodePlayAudio: {
  //play audio PCM buffer
  BufferParam* audioBuffer = (BufferParam*)param;
  PlayAudio(audioBuffer);
  statusCode = StatusCodeOk;
  break;
}
```

### Settings

All available camera settings (fields and limits) described in features json file, passed in initialization step.
SDK controls current settings with get/set codes in control function and params with corresponding types. 
List of settings codes

```
ControlCodeMicrophoneGet,
ControlCodeMicrophoneSet,

ControlCodeNightmodeGet,
ControlCodeNightmodeSet,

ControlCodeTimezoneGet,
ControlCodeTimezoneSet,

ControlCodeImageRotationGet,
ControlCodeImageRotationSet,

ControlCodeImageMirrorGet,
ControlCodeImageMirrorSet,

ControlCodePtzGet,
ControlCodePtzSet,

ControlCodeMotionDetectionGet,
ControlCodeMotionDetectionSet,

ControlCodeHumanDetectionGet,
ControlCodeHumanDetectionSet,

ControlCodeVehicleDetectionGet,
ControlCodeVehicleDetectionSet,

ControlCodeLineCrossingDetectionGet,
ControlCodeLineCrossingDetectionSet,

ControlCodeIntrusionDetectionGet,
ControlCodeIntrusionDetectionSet,

ControlCodeNetworkGet
```

For example, microphone settings 

```
case ControlCodeMicrophoneGet: {
    SettingMicrophone* microphone = (SettingMicrophone*)param;
    microphone->enable = true;
    microphone->sensitivity = 45;
    break;
}
case ControlCodeMicrophoneSet: {
    SettingMicrophone* microphone = (SettingMicrophone*)param;
    //set microphone param
    SetMicrophoneSetting(microphone->enable, microphone->sensitivity);
    break;
}
```
if setting not supported status code **StatusCodeNotSupported** must be returned.

Motion detection areas can be one of three types: matrix, rects or polygons. Setting description will vary depending on type

```
case ControlCodeHumanDetectionSet: {
    SettingHumanDetection* humanDetection = (SettingHumanDetection*)param;
    //set human detection params
    if (humanDetection->area.rectsCount > 0) {
        for (size_t i = 0; i < humanDetection->area.rectsCount; i++) {
            SettingAreaRect rect = humanDetection->area.rects[i];
            //rect.x, rect.y, rect.w, rect.h;
        }
    } else if (humanDetection->area.matrixCellsCount > 0) {
        for (size_t i = 0; i < humanDetection->area.matrixCellsCount; i++) {
            SettingAreaMatrixCell cell = humanDetection->area.matrix[i];
            //cell.value;
        }
    } else if (humanDetection->area.polygonsCount > 0) {
        for (size_t i = 0; i < humanDetection->area.polygonsCount; i++) {
            SettingAreaPolygon polygon = humanDetection->area.polygons[i];
            for (size_t j = 0; j < polygon.pointsCount; j++) {
                SettingAreaPolygonPoint point = polygon.points[j];
                //point.x, point.y;
            }
            
        }
    }
    break;
}
 ```   

### Motion detection events

When Motion Detector on camera detects motions events, they should be passed to library with `FaceterClientOnVideoEvent`.
```
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

typedef struct DetectionGrid 
{
    int rowsCount;
    int colsCount;
    int cellsCount;
    uint32_t *cells;
} DetectionGrid;

void FaceterClientOnVideoEventStart(int64_t eventTimestampSec, VideoEventType eventType, ObjectType objectType, 
    DetectionAttribute *attributesList, DetectionGrid detectionGrid, 
    char* snapshotImage, long int snapshotBytesCount);

void FaceterClientOnVideoEventUpdate(int64_t eventTimestampSec, VideoEventType eventType, ObjectType objectType, 
    DetectionGrid detectionGrid);    

void FaceterClientOnVideoEventEnd(int64_t eventTimestampSec, VideoEventType eventType, ObjectType objectType, 
    DetectionGrid detectionGrid);
```
Long time detection events have start point, end point and can be updated between start and end.
Use **FaceterClientOnVideoEventStart**, **FaceterClientOnVideoEventEnd** and **FaceterClientOnVideoEventUpdate** respectively.
Pass event timestamp in seconds since 01.01.1970. Each video event has VideoEventType if can be recognized, otherwise use **VideoEventMotion**. If object can be detected it's type passed as second parameters, otherwise use **ObjectUnknown**.

Also detector may provide object's attributes, that can be passed linked list of **DetectionAttribute**.
Parameter can be NULL if no information about object attributes provided.
DetectionAttribute is key-value structure of string type. Use PushDetectionAttribute to add object attribute.
Helper function PushHumanAttibutes creates DetectionAttribute list for human with gender and age fields.
Helper function PushVehicleAttributes creates DetectionAttribute list for vehicle with type and license plate fields

For describing cells in grid where object was detected pass DetectionGrid object. It has number of columns, rows and array cells with uint32 numbers. Each bit in this number describe cell. If object detected in cell bit will be 1, otherwise 0.

Last two parameters - camera snapshot of detected event in jpeg format and snapshot bytes count. Can be NULL.

```
//get detected event snapshot
long int snapshotJpegBytesCount = 100;
char snapshotJpegImage[snapshotJpegBytesCount];

int64_t nowTime = (int64_t)time(NULL);
DetectionGrid grid = {
    .rowsCount = 10,
    .colsCount = 16,
    .cellsCount = 5
};
grid.cells = malloc(grid.cellsCount * sizeof(int32_t));
grid.cells[2] = 1;
//simple motion event started
FaceterClientOnVideoEventStart(nowTime, VideoEventMotion, ObjectUnknown, NULL, grid, snapshotJpegImage, snapshotJpegBytesCount);

//simple motion event updated periodically
FaceterClientOnVideoEventUpdate(nowTime1, VideoEventMotion, ObjectUnknown, grid1);
FaceterClientOnVideoEventUpdate(nowTime2, VideoEventMotion, ObjectUnknown, grid2);

//simple motion event ended
FaceterClientOnVideoEventEnd(nowTime3, VideoEventMotion, ObjectUnknown, grid3);

//human motion event
DetectionAttribute* humanAttrList = NULL;
PushHumanAttibutes(&humanAttrList, GenderMale, 30);
DetectionRect* humanRect = NULL;
PushDetectionRect(&humanRect, 10, 15, 25, 49);
FaceterClientOnVideoEventStart(nowTime, VideoEventMotion, ObjectHuman, humanAttrList, grid, snapshotJpegImage, snapshotJpegBytesCount);
FaceterClientOnVideoEventUpdate(nowTime, VideoEventMotion, ObjectHuman, grid);
FaceterClientOnVideoEventEnd(nowTime, VideoEventMotion, ObjectHuman, grid);
```

One-time events can be passed to SDK with **FaceterClientOnVideoEvent** method

```
//animal motion event
DetectionAttribute* animalAttrList = NULL;
PushDetectionAttribute(&animalAttrList, "kind", "cat");
FaceterClientOnVideoEvent(VideoEventMotion, ObjectAnimal, animalAttrList, NULL, snapshotJpegImage, snapshotJpegBytesCount);

//line crossing event
DetectionRect* crossRects = NULL;
PushDetectionRect(&crossRects, 1, 5, 25, 49);
PushDetectionRect(&crossRects, 30, 45, 5, 17);
FaceterClientOnVideoEvent(VideoEventLineCrossing, ObjectOther, NULL, crossRects, snapshotJpegImage, snapshotJpegBytesCount);

//vehicle line crossing event
DetectionAttribute* vehicleAttrList = NULL;
PushVehicleAttributes(&vehicleAttrList, VehicleCar, "AB123");
FaceterClientOnVideoEvent(VideoEventLineCrossing, ObjectVehicle, vehicleAttrList, NULL, snapshotJpegImage, snapshotJpegBytesCount);

//loitering event
FaceterClientOnVideoEvent(VideoEventLoitering, ObjectHuman, NULL, NULL, NULL, 0);
```

Audio events from sound detector can be also send to library using `FaceterClientOnAudioEvent` with corresponding type.
If type cannot be recognized or not matches to AudioEventType use **AudioEventNoise**

```
typedef enum AudioEventType 
{
    AudioEventCry,
    AudioEventScream,
    AudioEventShot,
    AudioEventClap,
    AudioEventNoise
} AudioEventType;

//baby cry audio event
FaceterClientOnAudioEvent(AudioEventCry);
```

### Service functions

Application must provide these service functions if they supported:
+ **Jpeg snapshot** - library sometime needs camera preivew jpeg image. ControlHandler with code **ControlCodeGetSnapshot**
  will be called. In response application should call `FaceterClientOnSnapshot` with snapshot jpeg bytes array

  ```
  case ControlCodeGetSnapshot: {
      //get camera snapshot
      char* snapshotJpegImage = "";
      long int snapshotJpegBytesCount = 100;
      FaceterClientOnSnapshot(snapshotJpegImage, snapshotJpegBytesCount);
      break;
  }
  ```

+ **Registration reset** - if camera has _RESET_ button it can be used to reset registration state to initial.
   When button _RESET_ pressed longer than 3 seconds, apllication should call `FaceterClientReset` and reboot camera.

  ```
  /* 
  * Handler of reset button pressed more than 3 seconds
  */
  void OnResetButtonPressed() 
  {
      //reset registration
      FaceterClientReset();
      RebootSystem();
  }
  ```

  Also for resetting registration state library can call ControlFunction with **ControlCodeResetState**

  ```
  case ControlCodeResetState: {
      //reset registration state to initial
      OnResetButtonPressed();
      break;
  }
  ```
+ **Serial number** - unique serial number needed for camera identification.
  Application must provide serial number string that will be the same after camera restarted. If no serial number provided SDK will use MAC address for identification instead
+ **LED indication** - if camera has LED indicators they can be used to inform USER about current camera streaming state.
  Current state will be updated with ControlFunction code **ControlCodeStreamStatus**. If camera has two LED with different colors they should be used as follows
  ```
  case ControlCodeStreamStatus: {
     StreamStatus streamStatus = *(StreamStatus*)param;
     if (streamStatus == StreamStatusStarted) {
         //turn on constant green led
     } else if (streamStatus ==  StreamStatusInit) {
         //green led slowly blinking
     } else {
         //red led (or green led) rapidly blinking
     }
     break;
  }
  ```
  Where **green** LED is main indication color (could be any supported color) and **red** is additional color (if present)

+ **OTA firmware update** - after command from Faceter application SDK will call ControlFunction with code **ControlCodeUpdateFirmware** and URL to the file on server as param
  ```
  case ControlCodeUpdateFirmware: {
      //upgrade firmware URL
      const char* firmwareUpdateUrl = (const char*)param;
      break;
  }
  ```

## Library dependencies

Faceter CloudCam SDK depends on external libraries
* pthread
* json-c
* libCurl (with RTSP protocol enabled)
* embedTLS
