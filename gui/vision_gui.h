#ifndef GUI_VISION_UI_H_LF
#define GUI_VISION_UI_H_LF
#include <cstdint>
#include <string>
class QImage;


namespace gui{
typedef void (*Callback)(uint8_t, void*);
typedef void (*CallbackDisplayer)();

/**
 * @brief Start the GUI (This interface should be called in the main thread).
 * @param argc
 * @param argv
 * @return Application exec.
 */
int runVisionGUI(int argc, char* argv[]);


/**
 * @brief Register a callback functio to handle the GUI event.
 * @param callbackfunc
 */
void registerEventCallback(Callback callbackfunc);


/**
 * @brief Print message in the GUI
 * @param msg The message shown in the GUI
 */
void log(const std::string& msg);


/**
 * @brief The EventType for process GUI event.
 */
enum EventType
{
    EVENT_LOAD_LEFT_IMAGE,
    EVENT_LOAD_RIGHT_IMAGE,
    EVENT_LOAD_STEREO_IMAGE,
    EVENT_LOAD_LEFT_VIDEO,
    EVENT_LOAD_RIGHT_VIDEO,
    EVENT_LOAD_STEREO_VIDEO,

    EVENT_LOAD_CAM_PARAMS,
    EVENT_RECTIFY_STATUS,

    EVENT_CAMERA_STATUS,
    EVENT_CAM_FPS_STATUS,
    EVENT_CAPTURE,

    EVENT_CLOSE_DISPLAY,
    EVENT_PAUSE_DISPLAY,
    EVENT_GOON_DISPLAY,

    EVENT_ENHANCE_STATUS
};


/**
 * @brief The Capture struct for capturing image.
 */
struct CaptureInfo {
    bool        is_capture;
    uint8_t     save_num;
    std::string save_name;
    std::string save_path;

    bool		finished; // check whether saved all the images
};
extern CaptureInfo captureinfo;


} // namespace::gui

#endif // GUI_VISION_UI_H_LF
