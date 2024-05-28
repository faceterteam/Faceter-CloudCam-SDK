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
    case ControlCodeUpdateVideoEvent: {
        int64_t nowTime = (int64_t)time(NULL);
        DetectionGrid grid = {
            .rowsCount = 10,
            .colsCount = 16,
            .cellsCount = 5
        };
        grid.cells = malloc(grid.cellsCount * sizeof(int32_t));
        grid.cells[1] = 1234;
        FaceterClientOnVideoEventUpdate(nowTime,  VideoEventMotion, ObjectUnknown, grid);
        break;
    }
    case ControlCodeMicrophoneGet: {
        SettingMicrophone* microphone = (SettingMicrophone*)param;
        microphone->enable = true;
        microphone->sensitivity = 45;
        break;
    }
    case ControlCodeMicrophoneSet: {
        SettingMicrophone* microphone = (SettingMicrophone*)param;
        //set microphone param
        //SetMicrophoneSetting(microphone->enable, microphone->sensitivity);
        break;
    }
    case ControlCodeNightmodeGet: {
        SettingNightmode* nightmode = (SettingNightmode*)param;
        //get current night mode
        strcpy(nightmode->mode, SETTINGS_NIGHTMODE_AUTO);
        break;
    }
    case ControlCodeNightmodeSet: {
        SettingNightmode* nightmode = (SettingNightmode*)param;
        if (strcmp(nightmode->mode, SETTINGS_NIGHTMODE_ON) == 0) {
            //enable night mode
        } else if (strcmp(nightmode->mode, SETTINGS_NIGHTMODE_OFF) == 0) {
            //disable night mode
        } else {
            //set auto night mode
        }
        break;
    }
    case ControlCodeImageRotationGet: {
        SettingImageRotation* imageRotation = (SettingImageRotation*)param;
        //not supported setting example
        statusCode = StatusCodeNotSupported;
        break;
    }
    case ControlCodeImageRotationSet: {
        SettingImageRotation* imageRotation = (SettingImageRotation*)param;
        statusCode = StatusCodeNotSupported;
        break;
    }
    case ControlCodeMotionDetectionGet: {
        SettingMotionDetection* motionDetection = (SettingMotionDetection*)param;
        motionDetection->enable = true;
        motionDetection->threshold = 25;
        motionDetection->sensitivity = 45;
        //if area type is matrix
        motionDetection->area.matrixCellsCount = 3;
        motionDetection->area.matrix = malloc(motionDetection->area.matrixCellsCount * sizeof(SettingAreaMatrixCell));
        strcpy(motionDetection->area.matrix[0].value, "00aabcde");
        strcpy(motionDetection->area.matrix[1].value, "00001200");
        strcpy(motionDetection->area.matrix[2].value, "1e4f0000");
        break;
    }
    case ControlCodeMotionDetectionSet: {
        SettingMotionDetection* motionDetection = (SettingMotionDetection*)param;
        //set motion detection params;
        break;
    }
    case ControlCodeHumanDetectionGet: {
        SettingHumanDetection* humanDetection = (SettingHumanDetection*)param;
        humanDetection->enable = true;
        humanDetection->confidence = 60;
        // if area type is rects
        humanDetection->area.rectsCount = 1;
        humanDetection->area.rects = malloc(humanDetection->area.rectsCount * sizeof(SettingAreaRect));
        humanDetection->area.rects[0].x = 10;
        humanDetection->area.rects[0].y = 20;
        humanDetection->area.rects[0].w = 50;
        humanDetection->area.rects[0].h = 40;
        break;
    }
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
    case ControlCodeVehicleDetectionGet: {
        SettingVehicleDetection vehicle = {};
        vehicle.enable = true;
        vehicle.confidence = 50;
        strcpy(vehicle.scene, SETTINGS_SCENE_INDOOR);
        vehicle.area.polygonsCount = 1;
        vehicle.area.polygons = malloc(sizeof(SettingAreaPolygon));
        vehicle.area.polygons[0].pointsCount = 3;
        vehicle.area.polygons[0].points = malloc(3 * sizeof(SettingAreaPolygonPoint));
        vehicle.area.polygons[0].points[0].x = 10;
        vehicle.area.polygons[0].points[0].y = 15;
        vehicle.area.polygons[0].points[1].x = 20;
        vehicle.area.polygons[0].points[1].y = 30;
        vehicle.area.polygons[0].points[2].x = 100;
        vehicle.area.polygons[0].points[2].y = 50;

        *(SettingVehicleDetection*)param = vehicle;
        break;
    }
    case ControlCodeLineCrossingDetectionGet: {
        SettingLineCrossingDetection* lineCrossing = (SettingLineCrossingDetection*)param;
        lineCrossing->enable = true;
        lineCrossing->sensitivity = 30;
        lineCrossing->linesCount = 1;
        lineCrossing->lines = malloc(lineCrossing->linesCount * sizeof(SettingLine));
        lineCrossing->lines[0].ax = 5;
        lineCrossing->lines[0].ay = 5;
        lineCrossing->lines[0].bx = 45;
        lineCrossing->lines[0].by = 70;
        strcpy(lineCrossing->lines[0].direction, SETTINGS_LINE_DIRECTION_AB);
        break;
    }
    case ControlCodeNetworkGet: {
        SettingNetwork network = {};
        network.interfacesCount = 1;
        network.interfaces = malloc(network.interfacesCount * sizeof(SettingNetworkInterface));
        network.interfaces[0].active = true;
        strcpy(network.interfaces[0].name,"wlan0");
        network.interfaces[0].local = false;
        strcpy(network.interfaces[0].mac, "de:d0:11:00:aa");
        network.interfaces[0].ipv4.dhcp = true;
        strcpy(network.interfaces[0].ipv4.ip, "192.168.1.33");
        strcpy(network.interfaces[0].ipv4.mask, "255.255.255.0");
        strcpy(network.interfaces[0].ipv4.gateway, "192.168.1.1");
        strcpy(network.interfaces[0].ipv4.primaryDns, "8.8.8.8");
        strcpy(network.interfaces[0].ipv4.secondaryDns, "192.168.1.1");
        strcpy(network.interfaces[0].wifi.essid, "My WIFI network");
        strcpy(network.interfaces[0].wifi.signal, "-40");
        *(SettingNetwork*)param = network;
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