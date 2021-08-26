#ifndef IMAGE_READER_H_LF
#define IMAGE_READER_H_LF
#include <opencv2/opencv.hpp>
//#include <thread>
#include <string>
#include "../def/define.h"


#ifdef __linux__
class V4L2Capture;
#endif

/** @brief  the base class of monocular or binocular image reader
*/
class FrameReader
{
public:
    /**
     * @brief For active camera to grab frame stream.
     * @param usb_id        USB ID of the camera
     * @param cam_id        Camera ID
     */
    explicit FrameReader(uchar usb_id, uchar cam_id);

    /**
     * @brief For read frame stream form a specified video.
     * @param video_path
     */
    explicit FrameReader(const std::string& video_path);

    ~FrameReader();

    /**
    * @brief  Get current frame if there is one.
    */
    bool getFrame(cv::Mat & frame);

private:
    uchar   _usb_id;                        //!< the usb id of camera
    uchar   _cam_id;						//!< the cam id of camera

    uchar	_sharpness;						//!< the sharpness of the image

#ifdef __linux__
    V4L2Capture*    capture;                //!< used for capturing image in LINUX
#else // (defined _WIN64) || (defined __APPLE__)
    cv::VideoCapture    _video_capture;      //!< used for loading video in WINDOWS
#endif

};


#endif // IMAGE_READER_H_LF
