#include "vision_manager.h"
#include "camera/camera.h"
#include "camera/camera_parameters.h"
#include "image/frame_displayer.h"
#include "image/image_processor.h"
#include "def/cmd.h"
#include "def/ptr_define.h"

#ifdef __linux__
#include "usb/usb_device.h"
#include "usb/usb_camera_parameters.h"
#endif
#ifdef WITH_CUDA
#include <libvisiongpu/gpu_algorithm_pipeline_manager.h>

namespace
{
    void initGPUProcessor()
    {
        // Initialize the AlgoPipelineManager first
        gpu::AlgoPipelineManager::getInstance()->intialize();
        gpu::AlgoPipelineManager::getInstance()->addAlgoNode(new gpu::AlgoNodeNonuniform());
        gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::NonuniformProperty>(2, 0.99));
        gpu::AlgoPipelineManager::getInstance()->addAlgoNode(new gpu::AlgoNodeGamma());
        gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::GammaProperty>(0.005));
        gpu::AlgoPipelineManager::getInstance()->addAlgoNode(new gpu::AlgoNodeImageAdjust());
        gpu::AlgoPipelineManager::getInstance()->setProperty(std::make_shared<gpu::ImageAdjustProperty>(50, 60));
    }
}
#endif

uint16_t vision::ImageSize::width = 1920;
uint16_t vision::ImageSize::height = 1080;


VisionManager::VisionManager()
    : _cam_params_info("")
{
#ifdef WITH_CUDA
    initGPUProcessor();
#endif
    //_thread = std::thread(&VisionManager::show, this);
    //_thread.detach();
}

VisionManager::~VisionManager()
{

}

VisionManager* VisionManager::getInstance()
{
    static VisionManager manager;
    return &manager;
}

bool VisionManager::loadCamParams(std::string path)
{
    auto cam_params_reader = CameraParamsReaderFactory::getInstance()->create(path);

    // update the image size in vision_define.
    int image_width = 0, image_height = 0;
    cam_params_reader->getImageSize(image_width, image_height);
    if(image_width <= 0 || image_height <= 0) {
        LOG("Read image size failed!");
        return false;
    }
    else{
        vision::ImageSize::width = image_width;
        vision::ImageSize::height = image_height;
    }

    // initialize camera parameters
    auto cam_params = cam_params_reader->getStereoCameraParameters();
    if (!cam_params) {
        LOG("Read camera parametes failed.");
        return false;
    }
    ImageProcessor::getInstance()->setMapCalculator(std::make_shared<MapCalculator>(cam_params->left, image_width, image_height), vision::LEFT_CAMERA);
    ImageProcessor::getInstance()->setMapCalculator(std::make_shared<MapCalculator>(cam_params->right, image_width, image_height), vision::RIGHT_CAMERA);

    LOG("Read camera parameters successfully.");

    char info[64];
    sprintf(info, "View Camera Parameters:\nwidth,%d\nheight,%d\n", image_width, image_height);
    std::string cam_params_info = info + cam_params->getInfo();
    //DEBUG("%s\n", cam_params_info.c_str());
    LLOG(cam_params_info);

    return true;
}


void VisionManager::openCamera()
{
    camera_handle = CameraHandle::createInstance();
    camera_handle->openCamera();
}

void VisionManager::closeCamera()
{
    CameraHandle::releaseInstance(camera_handle);
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
        _image_displayer = std::make_shared<FrameDisplayer>("Display Window"); //FrameDisplayer::createInstance();
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
    //FrameDisplayer::releaseInstance(_image_displayer);
    _image_displayer.reset();
}



#if 0
bool CameraHandle::readCamParams()
{
    USBDevice usb_device;
    uint8_t left_data[MAX_DATA_SIZE];
    uint8_t right_data[MAX_DATA_SIZE];
    bool is_read_left = usb_device.readLeft(left_data);
    bool is_read_right = usb_device.readLeft(right_data);
    if(!is_read_left || !is_read_right)
    {
        printf("CameraHandle: Cannot read camera paramerters!\n");
        return false;
    }

    USBCameraParammeters usb_left_cam_params, usb_right_cam_params;
    memcpy(&usb_left_cam_params, left_data, sizeof(usb_left_cam_params));
    memcpy(&usb_right_cam_params, left_data, sizeof(usb_right_cam_params));
    if(!usb_left_cam_params.crc8Check() || !usb_right_cam_params.crc8Check())
    {
        printf("CameraHandle: Read failed camera paramerters!\n");
        return false;
    }
    uchar usb_id = usb_device.getLeftCamUSBIdx();
    uchar cam_id = uchar(vision::LEFT_CAMERA);
    //cameras[cam_id] = new Camera(usb_id, cam_id, usb_left_cam_params.width, usb_left_cam_params.width, CameraParameters(usb_left_cam_params));

    usb_id = usb_device.getRightCamUSBIdx();
    cam_id = uchar(vision::RIGHT_CAMERA);
    //cameras[cam_id] = new Camera(usb_id, cam_id, usb_right_cam_params.width, usb_right_cam_params.width, CameraParameters(usb_right_cam_params));

    printf("CameraHandle: Two camera initialized done.\n");

    return true;
}
#endif

/* ------------------------------------------------------------------------ */
/*                       VisionManager::CameraHandel                        */
/* ------------------------------------------------------------------------ */
VisionManager::CameraHandle::CameraHandle()
{
    for(int i = 0; i < vision::MAX_CAMERA_NUMBER; i++) {
        _cameras[i] = nullptr;
    }
}

VisionManager::CameraHandle::~CameraHandle()
{
    DELETE_ARRAY_PIONTER(_cameras, vision::MAX_CAMERA_NUMBER);
}

VisionManager::CameraHandle *VisionManager::CameraHandle::createInstance()
{
    return new CameraHandle();
}

void VisionManager::CameraHandle::releaseInstance(CameraHandle *handle)
{
    DELETE_PIONTER(handle);
}

void VisionManager::CameraHandle::openCamera()
{
    initCamera();
}

void VisionManager::CameraHandle::initCamera()
{
    uint8_t usb_id = 1;
    uchar cam_id = uchar(vision::LEFT_CAMERA);
    _cameras[cam_id] = new Camera(usb_id, cam_id, vision::ImageSize::width, vision::ImageSize::height);

    usb_id = 2;
    cam_id = uchar(vision::RIGHT_CAMERA);
    _cameras[cam_id] = new Camera(usb_id, cam_id, vision::ImageSize::width, vision::ImageSize::height);
    //LOG("Two camera initialized done.");
}





