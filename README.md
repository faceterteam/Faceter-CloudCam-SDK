# Faceter CloudCam SDK

## Overview

This SDK is designed to integrate IP cameras with the Faceter cloud video surveillance service. The library allows you to implement plug&play connection of cameras to the cloud and mobile applications, management of basic settings, transfer of AI detections and some service functions necessary for customer support

![Integration scheme](integration_scheme.png)

**Device App** is an application on the camera that implements the connection between the Faceter CloudCam SDK and the camera platform

**Platform SDK** is a camera manufacturer’s platform, it can be the original SDK from the chip manufacturer (SigmaStar, Ingenic, Goke, etc.), or the vendor’s middleware platform built on top of them

**Faceter CloudCam SDK** – modules that implement interaction with the cloud and Faceter applications. The SDK requires a local RTSP stream, which is transmitted to the Faceter cloud using RTSP-push technology

## Faceter CloudCam SDK prebuilt libraries

Prebuilt libraries for different platforms are located in the [latest release](https://github.com/faceterteam/Faceter-CloudCam-SDK/releases/latest)

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
    .cameraVendor = "Faceter",
    .appVersion = "1.0.0",
    .firmwareVersion = "Camera_1.0.3",
    .hardwareId = "t31_gc2053",
    .discoveryPort = 3702,
    .certFilePath = "/etc/ssl/certs/ca-certificates.crt",
    .confFilePath = "/etc/faceter-camera.conf",
    .rtspMainUrl = "rtsp://127.0.0.1/stream=0",
    .rtspCredentials = "root:12345",
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
* discoveryPort - port used for camera discovery from Faceter application (3702)
* certFilePath - path to the SSL certificate file, used in HTTPS connections ("/etc/ssl/certs/ca-certificates.crt")
* confFilePath - path to the file in the writable location where library will store it's state ("/etc/faceter-camera.conf")
* rtspMainUrl - main stream RTSP url without credentials ("rtsp://127.0.0.1/stream=0")
Library currently supports only video codec H264 and audio codec AAC
* rtspSubUrl - second RTSP stream, could be empty
* rtspCredentials - user name and password for acessing RTSP stream ("root:12345")

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

Faceter application can send commands to control camera paramets, such as microphone state (enabled or disabled).
Library will send control code ControlCodeMicrophone
```
//control handler fragment
...
case ControlCodeMicrophone: {
    //control microphone
    AudioConfig* audioConfig = (AudioConfig*)param;
    if (audioConfig->micEnabled) {
        //enable microphone on camera
    } else {
        //disable microphone on camera
    }
    break;
}
```
After operation completes application must call `FaceterClientSetControlStatus(controlCode, statusCode)` 
where statusCode is **StatusCodeOk** if operation succeed or other on fail.
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
    ObjectOther
} ObjectType;

void FaceterClientOnVideoEvent(VideoEventType eventType, ObjectType objectType, 
    DetectionAttribute *attributesList, DetectionRect *relativeBoundingRectList, 
    char* snapshotImage, long int snapshotBytesCount)
```
Each video event has VideoEventType if can be recognized, otherwise use **VideoEventMotion**. 
If object can be detected it's type passed as second parameters, otherwise use **ObjectOther**.

Also detector may provide object's attributes, that can be passed linked list of **DetectionAttribute**.
Parameter can be NULL if no information about object attributes provided.
DetectionAttribute is key-value structure of string type. Use PushDetectionAttribute to add object attribute.
Helper function PushHumanAttibutes creates DetectionAttribute list for human with gender and age fields.
Helper function PushVehicleAttributes creates DetectionAttribute list for vehicle with type and license plate fields

For describing objects bounding rects use **DetectionRect** list. Parameter can be NULL if no information about rect provided. 
Rect coordinates (top left corner, width and height) are relative to image size and must be set as integers in the range [0..99]. 
For adding next DetectionRect to list PushDetectionRect use PushDetectionRect

Last two parameters - camera snapshot of detected event in jpeg format and snapshot bytes count. Can be NULL.

```
//get detected event snapshot
long int snapshotJpegBytesCount = 100;
char snapshotJpegImage[snapshotJpegBytesCount];

//simple motion event
FaceterClientOnVideoEvent(VideoEventMotion, ObjectOther, NULL, NULL, NULL, 0);

//human motion event
DetectionAttribute* humanAttrList = NULL;
PushHumanAttibutes(&humanAttrList, GenderMale, 30);
DetectionRect* humanRect = NULL;
PushDetectionRect(&humanRect, 10, 15, 25, 49);
FaceterClientOnVideoEvent(VideoEventMotion, ObjectHuman, humanAttrList, humanRect, snapshotJpegImage, snapshotJpegBytesCount);

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

+ **OTA firmware update** - after command from Faceter application SDK will download and save firmware update 
  to '/tmp' directory and then call ControlFunction with code **ControlCodeUpdateFirmware** and path to the file as param
  ```
  case ControlCodeUpdateFirmware: {
      //upgrade firmware from file in tmp dir
      char* firmwareUpdate = (char*)param;
      break;
  }
  ```

## Library dependencies

Faceter CloudCam SDK depends on external libraries
* pthread
* json-c
* libCurl (with RTSP protocol enabled)
* embedTLS
