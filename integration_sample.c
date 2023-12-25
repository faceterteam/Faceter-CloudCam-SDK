#include <stdlib.h>
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
    //send motion detection event to library
    FaceterClientOnMotion();
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
        if (param != NULL) {
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
        long int snapshotJpegSize = 100;
        FaceterClientOnSnapshot(snapshotJpegImage, snapshotJpegSize);
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
        //upgrade firmware from file in temp dir or http url
        char* firmwareUpdate = (char*)param;
        break;
    }
    case ControlCodeRestartCamera: {
        if (param != NULL) {
            OnResetButtonPressed();
        } else {
            RebootSystem();
        }
        break;
    }
    case ControlCodeSetupWifi: {
        //setup wifi with ssid and password
        WifiConfig* config = (WifiConfig*)param;
        WifiSetup(config->network, config->password);
        break;
    }
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
    case ControlCodePlayAudio: {
        //play audio PCM buffer
        BufferParam* audioBuffer = (BufferParam*)param;
        statusCode = StatusCodeNotSupported;
        break;
    }
    default:
        statusCode = StatusCodeNotSupported;
        break;
    }

    FaceterClientSetControlStatus(controlCode, statusCode);
}

/* 
 * Setup video, audio and other supported params
 * video codec must be H264, audio codec AAC
 */
void ApplyCameraConfig(CameraConfig* cameraConfig) 
{
}

/*
 * Return unique stable serial number
 */
const char* GetSerialNumber() {
    return NULL;
}

int main() {
    const char* settingsPath = "/etc/faceter-client-settings.json";
    const char* serialNumber = GetSerialNumber();
    
    //library initialization
    if (FaceterClientInit(ControlHandler, settingsPath, serialNumber) < 0) {
        return 0;
    }
    
    //get camera config
    CameraConfig* cameraConfig = FaceterClientGetCameraConfig();
    //setup video, audio and other supported params
    ApplyCameraConfig(cameraConfig);

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