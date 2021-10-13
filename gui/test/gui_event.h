#ifndef VISION_GUI_EVENT_H_LF
#define VISION_GUI_EVENT_H_LF
#include "../vision_gui.h"
#include <string>
#include <cstdio>

void onGUIEvent(unsigned char id, void* data)
{
    std::string path;
    switch(id){
    case gui::EVENT_LOAD_LEFT_IMAGE:
        path = *static_cast<std::string*>(data);
        printf("id: %d, data:[%s]\n", id, path.c_str());
        break;
    case gui::EVENT_LOAD_RIGHT_IMAGE:
        path = *static_cast<std::string*>(data);
        printf("id: %d, data:[%s]\n", id, path.c_str());
        break;
    case gui::EVENT_LOAD_STEREO_IMAGE:
        path = *static_cast<std::string*>(data);
        printf("id: %d, data:[%s]\n", id, path.c_str());
        break;
    case gui::EVENT_LOAD_LEFT_VIDEO:
        path = *static_cast<std::string*>(data);
        printf("id: %d, data:[%s]\n", id, path.c_str());
        break;
    case gui::EVENT_LOAD_RIGHT_VIDEO:
        path = *static_cast<std::string*>(data);
        printf("id: %d, data:[%s]\n", id, path.c_str());
        break;
    case gui::EVENT_LOAD_STEREO_VIDEO:
        path = *static_cast<std::string*>(data);
        printf("id: %d, data:[%s]\n", id, path.c_str());
        break;
    case gui::EVENT_CLOSE_DISPLAY:
        printf("Close display.\n");
        break;
    case gui::EVENT_PAUSE_DISPLAY:
        printf("Pause display.\n");
        break;
    case gui::EVENT_GOON_DISPLAY:
        printf("Go on display.\n");
        break;
    case gui::EVENT_RECTIFY_STATUS:
        printf("Do rectify status: %d.\n", *static_cast<bool*>(data));
        break;
    case gui::EVENT_LOAD_CAM_PARAMS:
        path = *static_cast<std::string*>(data);
        printf("CamParams: id: %d, data:[%s]\n", id, path.c_str());
        break;
    case gui::EVENT_CAMERA_STATUS:
        printf("Open caemra status: %d.\n", *static_cast<bool*>(data));
        break;
    case gui::EVENT_CAM_FPS_STATUS:
        printf("Show FPS status: %d.\n", *static_cast<bool*>(data));
        break;
    case gui::EVENT_CAPTURE:
        printf("Captureinfo: N:%d, name:%s, path:%s.\n",
               gui::captureinfo.save_num, gui::captureinfo.save_name.c_str(),
               gui::captureinfo.save_path.c_str());
        break;
    case gui::EVENT_ENHANCE_STATUS:
        printf("Do enhance status: %d.\n", *static_cast<bool*>(data));
        break;
    default:
        break;
    }
}

#endif // VISION_GUI_EVENT_H_LF
