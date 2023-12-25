# Faceter CloudCam SDK

## Overview

This SDK is designed to integrate IP cameras with the Faceter cloud video surveillance service. The library allows you to implement plug&play connection of cameras to the cloud and mobile applications, management of basic settings, transfer of AI detections and some service functions necessary for customer support

![Integration scheme](integration_scheme.png)

**Device App** is an application on the camera that implements the connection between the Faceter CloudCam SDK and the camera platform

**Platform SDK** is a camera manufacturer’s platform, it can be the original SDK from the chip manufacturer (SigmaStar, Ingenic, Goke, etc.), or the vendor’s middleware platform built on top of them

**Faceter CloudCam SDK** – modules that implement interaction with the cloud and Faceter applications. The SDK requires a local RTSP stream, which is transmitted to the Faceter cloud using RTSP-push technology.

## Step-by-Step Guide:

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

## Intergration sample

An example of code with stubs describing intergation process is shown in [integration_sample.c](integration_sample.c) 

## Initialization

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

### Settings file structure
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

## Registration

## Controlling camera

## Motion detection events

## Service functions



