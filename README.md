# Faceter CloudCam SDK

## Overview

This SDK is designed to integrate IP cameras with the Faceter cloud video surveillance service. The library allows you to implement plug&play connection of cameras to the cloud and mobile applications, management of basic settings, transfer of AI detections and some service functions necessary for customer support

![Integration scheme](integration_scheme.png)

**Device App** is an application on the camera that implements the connection between the Faceter CloudCam SDK and the camera platform
**Platform SDK** is a camera manufacturer’s platform, it can be the original SDK from the chip manufacturer (SigmaStar, Ingenic, Goke, etc.), or the vendor’s middleware platform built on top of them
**Faceter CloudCam SDK** – modules that implement interaction with the cloud and Faceter applications. The SDK requires a local RTSP stream, which is transmitted to the Faceter cloud using RTSP-push technology.

## Step-by-Step Guide:

1. Initialize library with FaceterClientInit 
2. Implement registration in Faceter cloud trough QR code scanning and WS-Discovery
3. Implement additional methods for controlling the camera via the cloud: 
   turning the microphone on/off, rotating the image, ...
4. Implement transmission of all detections supported by the camera
5. Implement service functions:
   * Jpeg snapshot
   * Registration reset
   * Serial number
   * LED indication

### Initialization

### Registration

### Controlling camera

### Motion detection events

### Service functions



