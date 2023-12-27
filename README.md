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

### Initialization

First step of integration process is library initialization

```
const char* settingsPath = "/etc/faceter-client-settings.json";
const char* serialNumber = GetSerialNumber();
    
//library initialization
if (FaceterClientInit(ControlHandler, settingsPath, serialNumber) < 0) {
     return 0;
}
```

FaceterClientInit accepts 3 parameters:

- **ControlFunction** - callback from library with control code and parameters
- **settingsPath** - path to the settings file in json format that will be parsed and validated
- **serialNumber** - unique device id, could be NULL (it will be generated from MAC address)

Returns 0 on success and -1 if some error value occurs

#### Settings file structure
An example of settings file is [faceter-client-settings.json](faceter-client-settings.json)

* rtspMainUrl - url of the RTSP stream from local RTSP server with user and password
  > "rtspMainUrl": "rtsp://root:12345@127.0.0.1/stream=0"
* rtspSubUrl - secondary stream url, could be empty
  > "rtspSubUrl": ""
* cameraModel - camera model name, shown in application
  > "cameraModel": "Faceter"
* firmwareSaveDir - writable location where firmware update will be downloaded
  > "firmwareSaveDir": "/tmp"
* endroidPort - port for internal library http server (optional, default 7654)
* discoveryPort - port for WS-Discovery (optional, default 3702)
* certFilePath - path to the CA certificate file, becase library uses HTTPS requests
  > "certFilePath": "/etc/ssl/certs/ca-certificates.crt"
* confFilePath - file where camera registration parameters will be stored (optional, default is dir where settings file located)
  > "/etc/faceter-camera.conf" 
* cameraConfig - describes video, audio and other settings. Can be used for setting up camera parameters
  * audio - audio config describes sample rate, codec, microphone and speaker activity 
  * mainStream - video config describes frame rate, codec, bitrate and image size of main stream
  * subStream - same for substream
  * image - image rotation
  * detector - motion detector state
  * osd - OSD visibility
  * nightMode - enable or disable night mode
* customConfig - here can be stored any other necessary settings in json format 

### Registration

WiFi cameras require QR scanner. Also connection to WiFi network with ssid and password should be implemented. Registration process in this case consists of 3 steps:
1. Library sends operation code **ControlCodeScanQr** with _not NULL_ parameter to start QR scanner
   
   ```
   case ControlCodeScanQr: {
     if (param != NULL) {
         //start QR code scanning
         QrScannerStart();
     } else {
         //stop QR code scanning
         QrScannerStop();
     }
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
   
4. If QR code is correct, code **ControlCodeScanQr** with _NULL_ parameter will be sent to stop QR scanner
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
  if (param != NULL) {
      //enable microphone on camera
  } else {
      //disable microphone on camera
  }
  break;
 }
```
After operation completes application must call `FaceterClientSetControlStatus(controlCode, statusCode)` 
where statusCode is **StatusCodeOk** if operation succeed or other on fail.
If operation not supported statusCode can be set to **StatusCodeNotSupported**. 
For example if camera not supports playing audio, it will return status without processing operation
```
case ControlCodePlayAudio: {
  //play audio PCM buffer
  BufferParam* audioBuffer = (BufferParam*)param;
  statusCode = StatusCodeNotSupported;
  break;
}
```

### Motion detection events

When Motion Detector on camera detects motions events, they should be passed to library with `FaceterClientOnMotion`

```
/*
 * Callback from Motion Detector
 */
void OnMotionDetected() 
{
    //send motion detection event to library
    FaceterClientOnMotion();
}
```

### Service functions

Application must provide these service functions if they supported:
+ **Jpeg snapshot** - library sometime needs camera preivew jpeg image. ControlHandler with code **ControlCodeGetSnapshot**
  will be called. In response application should call `FaceterClientOnSnapshot` with snapshot jpeg bytes array

   ```
   case ControlCodeGetSnapshot: {
     //get camera snapshot
     char* snapshotJpegImage = "";
     long int snapshotJpegSize = 100;
     FaceterClientOnSnapshot(snapshotJpegImage, snapshotJpegSize);
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

   Also for resetting registration state library can call ControlFunction with **ControlCodeRestartCamera** and _not NULL_ param

   ```
   case ControlCodeRestartCamera: {
     if (param != NULL) {
         OnResetButtonPressed();
     } else {
         RebootSystem();
     }
     break;
   }
   ```
+ **Serial number** - unique serial number needed for camera identification.
  Application must provide serial number string that will be the same after camera restarted. If no serial number provided camera will use MAC address as serial number
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

## Library dependencies

Faceter CloudCam SDK depends on external libraries
* pthread
* json-c
* libCurl (with RTSP protocol enabled)
* embedTLS
