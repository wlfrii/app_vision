/**
 * @file vision_manager.h
 * @author Longfei Wang (longfei.wang@sjtu.edu.cn)
 * @brief A class that control the construction and running of camera objecs.
 *     NOTE, this class will also control the construction for load images or
 * videos based on the UI commands.
 *
 * @version 1.0
 * @date 2021-08-19
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef VISION_MANAGER_H_LF
#define VISION_MANAGER_H_LF
#include "def/define.h"
#include <thread>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>

class Camera;
class FrameDisplayer;


class VisionManager
{
    class CameraHandle
    {
    protected:
        CameraHandle();
        ~CameraHandle();
    public:
        static CameraHandle* createInstance();
        static void releaseInstance(CameraHandle* handle);

        void openCamera();
    private:
        void initCamera();

        Camera* _cameras[vision::MAX_CAMERA_NUMBER];
    };


protected:
    VisionManager();
    virtual ~VisionManager();

public:
    static VisionManager* getInstance();

    bool loadCamParams(std::string path);

    void openCamera();
    void closeCamera();

    void handleImage(cv::Mat& image, uint8_t id);
    void showImage();
    void closeImageWindow();

    void handleVideo();

private:
    CameraHandle*   camera_handle;
    std::thread     _thread;

    std::string     _cam_params_info;

    cv::Mat _images[vision::MAX_CAMERA_NUMBER];
    cv::Mat _processed_images[vision::MAX_CAMERA_NUMBER];
    std::shared_ptr<FrameDisplayer> _image_displayer;
};

#endif // VISION_MANAGER_H_LF
