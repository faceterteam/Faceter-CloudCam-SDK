#include <stdlib.h>
#include <string.h>
#include "FaceterClient.h"

/*
 * Callback from QR scanner
 */
void OnQrScannerScanSuccess(char* qrCode) 
{
    //pass scanned string to Faceter library
    FaceterClientOnQrScanned(qrCode);
}

/*
 * Start QR code scanner
 */
void QrScannerStart() 
{
}

/*
 * Stop QR code scanner
 */
void QrScannerStop() 
{
}

/*
 * Callback from Motion Detector
 */
void OnMotionDetected() 
{
    //get motion event snapshot
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

    //baby cry audio event
    FaceterClientOnAudioEvent(AudioEventCry);
}

/*
 * Reboot device function
 */
void RebootSystem() 
{
}

/* 
 * Handler of reset button pressed more than 3 seconds
 */
void OnResetButtonPressed() 
{
    //reset registration
    FaceterClientReset();
    RebootSystem();
}

/*
 * Implement wifi setup for wireless cameras
 */
void WifiSetup(const char* networkSsid, const char* password) 
{
}


/*
 * Control operations handler
 */
void ControlHandler(ClientControlCode controlCode, void* param) 
{    
    ClientStatusCode statusCode = StatusCodeOk;

    switch (controlCode)
    {
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

    case ControlCodeRotateImage: {
        //rotate image
        ImageConfig *imageConfig = (ImageConfig*)param;
        if (imageConfig->isFlip && imageConfig->isMirror) {
            //rotate image at 180 degrees
        }
        break;
    }
    case ControlCodeGetSnapshot: {
        //get camera snapshot
        char* snapshotJpegImage = "";
        long int snapshotJpegBytesCount = 100;
        FaceterClientOnSnapshot(snapshotJpegImage, snapshotJpegBytesCount);
        break;
    }
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
    case ControlCodeUpdateFirmware: {
        //upgrade firmware from file in tmp dir
        char* firmwareUpdate = (char*)param;
        break;
    }
    case ControlCodeRebootCamera: {
        //reboot device
        RebootSystem();
        break;
    }
    case ControlCodeResetState: {
        //reset registration state to initial
        OnResetButtonPressed();
        break;
    }
    case ControlCodeSetupWifi: {
        //setup wifi with ssid and password
        WifiConfig* config = (WifiConfig*)param;
        WifiSetup(config->network, config->password);
        break;
    }
    case ControlCodeStartScanQr: {
        //start QR code scanning
        QrScannerStart();
        break;
    }
    case ControlCodeStopScanQr: {
        //stop QR code scanning
        QrScannerStop();
        break;
    }
    case ControlCodePlayAudio: {
        //play audio PCM buffer
        BufferParam* audioBuffer = (BufferParam*)param;
        statusCode = StatusCodeNotSupported;
        break;
    }
    default:
        statusCode = StatusCodeNotImplemented;
        break;
    }

    FaceterClientSetControlStatus(controlCode, statusCode);
}

/*
 * Return unique stable serial number
 */
const char* GetSerialNumber() {
    return NULL;
}

int main() {
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
    };
    strcpy(settings.serialNumber, serialNumber);
    
    //library initialization
    if (FaceterClientInit(ControlHandler, settings) < 0) {
        return 0;
    }

    //start main library logic
    FaceterClientStart();

    while (1) {
        //application logic here
        //sleep(60);
    }
    
    //stop streaming process
    FaceterClientStop();

    return 0;
}