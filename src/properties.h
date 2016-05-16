#include <iostream>
#define PROP_FILE_PATH "properties.properties"
#define DELAY_BEFORE_VIDEO_STARTED "delayBeforeVideoStarted"
#define DELAY_AFTER_VIDEO_FINISHED "delayAfterVideoFinished"
#define VIDEO_DIR "fullVideoDirectory"
#define DELAY_BEFORE_CLOSE_WEBCAM "delayBeforeCloseWebcam"
#define WEBCAM_SOURCE "webcamSource"
#define DELAY_AFTER_WEBCAM_CLOSES "delayAfterWebcamCloses"
#define AMBIENT_BRIGHTNESS_THRESHOLD "ambientBrightnessThreshold"
#define IS_FULL_SCREEN "isFullScreen"

using namespace std;

int initializeProperties(string);
string findPropertyValueByKey(string);
