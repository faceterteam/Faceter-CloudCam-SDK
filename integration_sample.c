#include <stdlib.h>
#include "FaceterClient.h"

void RebootSystem() {
    //reboot device
}

void QrScannerStart(void (*scanCompleteCallback)(const char *qr)) {
    //start QR code scanning
    char* qrCodeString = "";
    scanCompleteCallback(qrCodeString);
}

void QrScannerStop() {
    //stop QR code scanning
}

void StartMotionDetector(void (*motionDetectedCallback)()) {
    //start motion detection
}

void StopMotionDetector() {
    //stop motion detection
}

void RotateImage(ImageConfig* config) {
    if (config->isFlip && config->isMirror) {
        //rotate image at 180 degrees
    }
}

void ControlHandler(ClientControlCode code, void* param) {
    
    ClientStatusCode statusCode = StatusCodeOk;

    switch (code)
    {
    case ControlCodeMotionDetector: {
        //enable/disable motion detector
        DetectorConfig *detector = (DetectorConfig*)param;
        if (detector->isEnabled) {
            StartMotionDetector(FaceterClientOnMotion);
        } else {
            StopMotionDetector();
        }
        break;
    }
    case ControlCodeMicrophone: {
        //change audio params - enable/disable microphone
        AudioConfig *audio = (AudioConfig*)param;
        break;
    }
    case ControlCodeNightMode: {
        //enable/disable night mode
        NightModeConfig *nightMode = (NightModeConfig*)param;
        //on feature applying error
        statusCode = StatusCodeFail;
        break;
    }
    case ControlCodeOsd: {
        //enable/disable osd
        OsdConfig *osd = (OsdConfig*)param;
        //if feature not implemented
        statusCode = StatusCodeNotSupported;
        break;
    }
    case ControlCodeRotateImage: {
        //rotate image
        ImageConfig *image = (ImageConfig*)param;
        RotateImage(image);
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
        StreamStatus* streamStatus = (StreamStatus*)param;
        //stream status changed - need to signal user (for example: led blink)
        break;
    }
    case ControlCodeUpdateFirmware: {
        char* fileName = (char*)param;
        //upgrade firmware from file in temp dir
        break;
    }
    case ControlCodeRestartCamera: {
        if (param != NULL) {
            FaceterClientReset();
            RebootSystem();
        } else {
            RebootSystem();
        }
        break;
    }
    case ControlCodeSetupWifi: {
        //setup wifi with ssid and password
        WifiConfig* config = (WifiConfig*)param;
        break;
    }
    case ControlCodeScanQr: {
        if (param != NULL) {
            //start QR code scanning
            QrScannerStart(FaceterClientOnQrScanned);
        } else {
            //stop QR code scanning
            QrScannerStop();
        }
        break;
    }
    case ControlCodePlayAudio: {
        //play audio PCM buffer
        BufferParam* audioBuffer = (BufferParam*)param;
        break;
    }
    default:
        statusCode = StatusCodeNotSupported;
        break;
    }

    FaceterClientSetControlStatus(code, statusCode);
}

void ApplyCameraConfig(CameraConfig* cameraConfig) {
    //setup video, audio and other supported params
}

int main() {
    const char* settingsPath = "/etc/faceter-client-settings.json";
    
    //library initialization
    if (FaceterClientInit(ControlHandler, settingsPath, NULL) < 0) {
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