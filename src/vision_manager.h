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
#include "util/define.h"
#include <thread>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>

class Camera;
class FrameDisplayer;


class VisionManager
{
protected:
    VisionManager();
    virtual ~VisionManager();

public:
    static VisionManager* getInstance();


    bool loadCamParams(std::string path);

    /**
     * @brief Open binocular camera
     * @param l_usb_id
     * @param r_usb_id
     */
    void openCamera(uint8_t l_usb_id = 0, uint8_t r_usb_id = 1);

    /**
     * @brief Close camera
     */
    void closeCamera();

    /**
     * @brief Load video and display it.
     * @param video_path
     * @param id
     */
    void handleVideo(std::string video_path, uint8_t id);

    // Handle image window
    void handleImage(cv::Mat& image, uint8_t id);
    void showImage();
    void closeImageWindow();

private:
    Camera* _cameras[vision::MAX_CAMERA_NUMBER];

    cv::Mat _images[vision::MAX_CAMERA_NUMBER];
    cv::Mat _processed_images[vision::MAX_CAMERA_NUMBER];
    std::shared_ptr<FrameDisplayer> _image_displayer;
};

#endif // VISION_MANAGER_H_LF
