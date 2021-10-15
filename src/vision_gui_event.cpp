#include "vision_ext.h"
#include "util/util.h"
#ifdef WITH_QT_GUI
#include "../gui/vision_gui.h"
#include "camera/camera_parameters.h"
#include <string>

void LOG(const std::string& msg)
{
    //gui::log(msg);
}

enum MediaType { LEFT, RIGHT, STEREO };
enum DisplayType{ CLOSE, PAUSE, GO_ON };

void onLoadImageEvent(const std::string&, MediaType);
void onLoadVideoEvent(const std::string&, MediaType);
void onDisplayEvent(DisplayType);
void onLoadCamParamsEvent(const std::string&);
void onRectifyEvent(bool);
void onCameraEvent(bool);
void onFPSEvent(bool);
void onCaptureEvent(bool);
void onEnhanceEvent(bool);


void onGUIEvent(unsigned char id, void* data)
{
    switch(id){
    case gui::EVENT_LOAD_LEFT_IMAGE:
        onLoadImageEvent(*static_cast<std::string*>(data), LEFT);
        break;
    case gui::EVENT_LOAD_RIGHT_IMAGE:
        onLoadImageEvent(*static_cast<std::string*>(data), RIGHT);
        break;
    case gui::EVENT_LOAD_STEREO_IMAGE:
        onLoadImageEvent(*static_cast<std::string*>(data), STEREO);
        break;
    case gui::EVENT_LOAD_LEFT_VIDEO:
        onLoadVideoEvent(*static_cast<std::string*>(data), LEFT);
        break;
    case gui::EVENT_LOAD_RIGHT_VIDEO:
        onLoadVideoEvent(*static_cast<std::string*>(data), RIGHT);
        break;
    case gui::EVENT_LOAD_STEREO_VIDEO:
        onLoadVideoEvent(*static_cast<std::string*>(data), STEREO);
        break;
    case gui::EVENT_LOAD_CAM_PARAMS:
        onLoadCamParamsEvent(*static_cast<std::string*>(data));
        break;
    case gui::EVENT_RECTIFY_STATUS:
        onRectifyEvent(*static_cast<bool*>(data));
        break;
    case gui::EVENT_CAMERA_STATUS:
        onCameraEvent(*static_cast<bool*>(data));
        break;
    case gui::EVENT_CAM_FPS_STATUS:
        onFPSEvent(*static_cast<bool*>(data));
        break;
    case gui::EVENT_CAPTURE:
        onCaptureEvent(*static_cast<bool*>(data));
        break;
    case gui::EVENT_CLOSE_DISPLAY:
        onDisplayEvent(CLOSE);
        break;
    case gui::EVENT_PAUSE_DISPLAY:
        onDisplayEvent(PAUSE);
        break;
    case gui::EVENT_GOON_DISPLAY:
        onDisplayEvent(GO_ON);
        break;
    case gui::EVENT_ENHANCE_STATUS:
        onEnhanceEvent(*static_cast<bool*>(data));
        break;
    default:
        break;
    }
}

/* -------------------------------------------------------------------------- */
void onLoadImageEvent(const std::string& path, MediaType type)
{

}

void onLoadVideoEvent(const std::string& path, MediaType type)
{

}

void onDisplayEvent(DisplayType type)
{

}

void onLoadCamParamsEvent(const std::string& path)
{

}

void onRectifyEvent(bool is_rectify)
{

}

void onCameraEvent(bool is_open)
{

}

void onFPSEvent(bool is_show)
{

}

void onCaptureEvent(bool is_capture)
{
//    printf("Captureinfo: N:%d, name:%s, path:%s.\n",
//           gui::captureinfo.save_num, gui::captureinfo.save_name.c_str(),
//           gui::captureinfo.save_path.c_str());
}

void onEnhanceEvent(bool is_enhance)
{

}

#else
#include <cstdio>

void LOG(const std::string& msg)
{
    printf("%s\n", msg.c_str());
}

#endif // WITH_QT_GUI

void activateGUIEvent()
{
#ifdef WITH_QT_GUI
    //gui::registerEventCallback(onGUIEvent);
#endif
}
