#ifndef CONTROLFUNCTION_H
#define CONTROLFUNCTION_H

/*
 * Operation control code passed through ControlFunction
 */
typedef enum ClientControlCode {
    ControlCodeMicrophone,          /**< enable/disable microphone */
    ControlCodeNightMode,           /**< enable/disable night mode */
    ControlCodeMotionDetector,      /**< enable/disable motion detector */
    ControlCodeOsd,                 /**< enable/disable osd */
    ControlCodeRotateImage,         /**< set rotation params */
    ControlCodeRestartCamera,       /**< reboot/reset camera */
    ControlCodeUpdateFirmware,      /**< update firmware from file */
    ControlCodeGetSnapshot,         /**< take camera snapshot */
    ControlCodePlayAudio,           /**< play audio PCM bytes buffer  */
    ControlCodeStreamStatus,        /**< update stream status */
    ControlCodeSetupWifi,           /**< set wifi configuration from param */
    ControlCodeScanQr               /**< enable/disable qr code scanner */
} ClientControlCode;

/*
 * Operation return status code passed through FaceterClientSetControlStatus
 */
typedef enum ClientStatusCode {
    StatusCodeOk,                   /**< control code operation succeed */
    StatusCodeFail,                 /**< control code operation failed */
    StatusCodeTimeout,              /**< control code operation timeout */
    StatusCodeInvalidParam,         /**< control code operation with invalid param */
    StatusCodeNotSupported          /**< control code operation not supported */
} ClientStatusCode;

/*
 * Stream status passed as param with code ControlCodeStreamStatus
 */
typedef enum StreamStatus {
    StreamStatusInit,               /**< stream not initialized */
    StreamStatusStopped,            /**< stream initialized and not started yet */
    StreamStatusStarted,            /**< stream successfully started */
    StreamStatusStartError,         /**< stream start error */
    StreamStatusConnectError        /**< stream connection error */
} StreamStatus;

typedef struct BufferParam {
    const char* data;               /**< data buffer bytes */
    long int dataSize;              /**< data buffer size */
} BufferParam;

/*
 * Controlling callback
 * 
 * @param code operation control code
 * @param param pointer to operation parameter, depended on code 
 */
typedef void (*ControlFunction) (ClientControlCode code, void* param);

#endif