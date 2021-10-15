#include "vision_manager.h"
#include "camera/camera.h"
#include "camera/camera_parameters.h"
#include "image/frame_displayer.h"
#include "image/image_processor.h"

#include "vision_ext.h"
#include "util/util.h"
#include "util/define.h"

namespace vision
{

double camera_distance = 0;
double camera_fov = 0;

uint16_t ImageSize::width = 1920;
uint16_t ImageSize::height = 1080;

const int FRAME_UPDATE_INTERVAL_MS = 16;
const int MAX_CAPTURE_TIME_MS = 500;

}


VisionManager::VisionManager()
{
    for(int i = 0; i < vision::MAX_CAMERA_NUMBER; i++) {
        _cameras[i] = nullptr;
    }

    /** Make the necessary initialization before start vision.
     * Particularly, the callback functio for vision gui should be registered
     * firstly, and the gpu process also need presets (in the future function).
     */
    activateGUIEvent();
    initGPUPipeline();
}

VisionManager::~VisionManager()
{
    closeCamera();
}

VisionManager* VisionManager::getInstance()
{
    static VisionManager manager;
    return &manager;
}


bool VisionManager::loadCamParams(std::string path)
{
    auto cam_params_reader = CameraParamsReaderFactory::getInstance()->create(path);

    // initialize camera parameters
    auto cam_params = cam_params_reader->getStereoCameraParameters();
    if (!cam_params) {
        LOG("Read camera parametes failed.");
        return false;
    }

    // check image size
    auto L_roi = cam_params_reader->getStereoCameraParameters()->left->getROI();
    auto R_roi = cam_params_reader->getStereoCameraParameters()->right->getROI();
    if(L_roi.width  != R_roi.width || L_roi.height  != R_roi.height) {
        LOG("ERROR: The frame size for stereo image is difference!");
        return false;
    }

    vision::camera_fov = cam_params_reader->getCamFOV();
    vision::camera_distance = cam_params_reader->getCamDistance();

    ImageProcessor::getInstance()->setMapCalculator(
        std::make_shared<MapCalculator>(cam_params->left), vision::LEFT_CAMERA);
    ImageProcessor::getInstance()->setMapCalculator(
        std::make_shared<MapCalculator>(cam_params->right), vision::RIGHT_CAMERA);

    LOG("Read camera parameters successfully.");

    char info[64];
    sprintf(info, "View Camera Parameters:fov:%f\nt:%f\n", vision::camera_fov, vision::camera_distance);
    std::string cam_params_info = info + cam_params->getInfo();
    LOG(cam_params_info);

    return true;
}


void VisionManager::openCamera(uint8_t l_usb_id, uint8_t r_usb_id)
{
    closeCamera();
    uchar cam_id = uchar(vision::LEFT_CAMERA);
    _cameras[cam_id] = new Camera(cam_id, l_usb_id);

    cam_id = uchar(vision::RIGHT_CAMERA);
    _cameras[cam_id] = new Camera(cam_id, r_usb_id);
    LOG("Two camera initialized done.");
}


void VisionManager::closeCamera()
{
    DELETE_ARRAY_PIONTER(_cameras, vision::MAX_CAMERA_NUMBER);
}


void VisionManager::handleVideo(std::string video_path, uint8_t id)
{
    DELETE_PIONTER(_cameras[id]);
    _cameras[id] = new Camera(id, video_path);
}


void VisionManager::handleImage(cv::Mat &image, uint8_t id)
{
    if(id >= vision::MAX_CAMERA_NUMBER){
        return;
    }
    image.copyTo(_images[id]);
}


void VisionManager::showImage()
{
    if(!_image_displayer){
        _image_displayer = std::make_shared<FrameDisplayer>(
                    vision::ImageSize::width, vision::ImageSize::height);
    }

    for(int i = 0; i < vision::MAX_CAMERA_NUMBER; i++){
        if(!_images[i].empty()){
            ImageProcessor::getInstance()->processImage(_images[i], _processed_images[i], i);
            _image_displayer->updateFrame(_processed_images[i], i);
        }
    }
    _image_displayer->showFrame();
}

void VisionManager::closeImageWindow()
{
    _image_displayer.reset();
    for(int i = 0; i < vision::MAX_CAMERA_NUMBER; i++){
        _images[i].release();
    }
}








