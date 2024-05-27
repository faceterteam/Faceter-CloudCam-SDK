#include <stdlib.h>
#include <string.h>
#include <time.h>
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
    FaceterClientOnVideoEventUpdate(nowTime, VideoEventMotion, ObjectUnknown, grid);
    FaceterClientOnVideoEventUpdate(nowTime, VideoEventMotion, ObjectUnknown, grid);

    //simple motion event ended
    FaceterClientOnVideoEventEnd(nowTime, VideoEventMotion, ObjectUnknown, grid);

    //human motion event
    DetectionAttribute* humanAttrList = NULL;
    PushHumanAttibutes(&humanAttrList, GenderMale, 30);
    DetectionRect* humanRect = NULL;
    PushDetectionRect(&humanRect, 10, 15, 25, 49);
    FaceterClientOnVideoEventStart(nowTime, VideoEventMotion, ObjectHuman, humanAttrList, grid, snapshotJpegImage, snapshotJpegBytesCount);
    FaceterClientOnVideoEventUpdate(nowTime, VideoEventMotion, ObjectHuman, grid);
    FaceterClientOnVideoEventEnd(nowTime, VideoEventMotion, ObjectHuman, grid);

    //animal motion event
    DetectionAttribute* animalAttrList = NULL;
    PushDetectionAttribute(&animalAttrList, "kind", "cat");
    FaceterClientOnVideoEvent(nowTime, VideoEventMotion, ObjectAnimal, animalAttrList, grid, snapshotJpegImage, snapshotJpegBytesCount);

    //line crossing event
    grid.cells[1] = 12345;
    FaceterClientOnVideoEvent(nowTime, VideoEventLineCrossing, ObjectOther, NULL, grid, snapshotJpegImage, snapshotJpegBytesCount);

    //vehicle line crossing event
    DetectionAttribute* vehicleAttrList = NULL;
    PushVehicleAttributes(&vehicleAttrList, VehicleCar, "AB123");
    FaceterClientOnVideoEvent(nowTime, VideoEventLineCrossing, ObjectVehicle, vehicleAttrList, grid, snapshotJpegImage, snapshotJpegBytesCount);

    //loitering event
    FaceterClientOnVideoEvent(nowTime, VideoEventLoitering, ObjectHuman, NULL, grid, NULL, 0);

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
ClientStatusCode ControlHandler(ClientControlCode controlCode, void* param) 
{    
    ClientStatusCode statusCode = StatusCodeOk;

    switch (controlCode)
    {
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

    return statusCode;
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