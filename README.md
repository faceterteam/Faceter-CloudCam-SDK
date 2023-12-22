# Avalab.VideoS.FaceterClient

Cross-platform client library for Faceter Camera

## Integration GUIDE:

### Library Settings

Client settings desribed in the file **faceter-client-settings.json** 
and must be located at writable location (for example /etc)

Settings stored in JSON format, where:

* rtspMainUrl - url of the RTSP stream from local RTSP server
* rtspSubUrl - secondary stream url, could be empty
* cameraModel - camera model name
* firmwareSaveDir - location where firmware update will be downloaded
* resourcesDir - location of the app resources (such as audio files)
* cameraConfig - desribes video, audio and other settings
  * audio - audio config describes sample rate, codec, microphone and speaker activity 
  * mainStream - video config describes frame rate, codec, bitrate and image size of main stream
  * subStream - same for substream
  * image - image rotation
  * detector - motion detector state
  * osd - OSD visibility
  * nightMode - enable or disable night mode
* customConfig - here can be stored any other necessary settings in json format 

### Library dependencies

FaceterCLient depends on pthreads, libcurl and json-c libraries. Libcurl depends on mbedtls library
In CMake file options

    option(FACETER_BUILD_SHARED_LIBS  "Default to building shared libraries" OFF)

Option **FACETER_BUILD_SHARED_LIBS** could be set for building shared or static library 

    option(FETCH_DEPS  "Build libraries for dependencies" ON)

Option **FETCH_DEPS** when enabled is used for building dependency libraries from sources 

Application CMake example, building static library with installed dependencies

    option(FACETER_BUILD_SHARED_LIBS  "Default to building shared libraries" OFF)
    option(FETCH_DEPS  "Build libraries for dependencies" OFF)

    ADD_SUBDIRECTORY(faceterclient)

    add_executable(${PROJECT_NAME} ${SOURCES})

    target_include_directories(${PROJECT_NAME} PRIVATE "faceterclient/FaceterClient")

    target_link_libraries(${PROJECT_NAME} faceter-client)

### Integration sample

Sample file located at **FaceterClient/integration_sample.c**

1. First step is library initialization

        if (FaceterClientInit(ControlHandler, settingsPath) < 0) {
            return 0;
        }

    * *settingsPath* - path to the faceter-client-settings.json file
    * *ControlHandler* - is function pointer to library commands handler

2. Applying camera settings, parsed from settings file

        //get camera config
        CameraConfig* cameraConfig = FaceterClientGetCameraConfig();
        //setup video, audio and other supported params
        ApplyCameraConfig(cameraConfig);

3. Library setting up and streaming

        //start main library logic
        FaceterClientStart();

#### Control handler

Control handler is callback that library calls when it needs to do some external action

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
            ...

            FaceterClientSetControlStatus(code, statusCode);
        }

Parameter **param** depends on code value. ClientControlCode is what action app must do. 
After action complete *FaceterClientSetControlStatus* should be called. 
On success with status code **StatusCodeOk**, on error **StatusCodeFail**, if action could not be done **StatusCodeNotSupported**

Application **must** implement QR scanning, Jpeg snapshot and Wifi Setup actions (for wireless cameras). After QR scan is complete **FaceterClientOnQrScanned** must be called. After obtaininf of current jpeg image **FaceterClientOnSnapshot** must be called


