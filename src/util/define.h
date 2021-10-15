#pragma once
#include <chrono>
#include <opencv2/opencv.hpp>
#include <stdio.h>


/** DEBUG = printf **/
#define DEBUG(fmt, ...) \
    printf("[%s][%s][%d] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)



namespace vision
{
    extern double camera_distance;
    extern double camera_fov;

    /** @brief This struct is use to specify the size of image displaying.
     * The default size is set to 1080x1920.
     */
    struct ImageSize
    {
        static uint16_t width;
        static uint16_t height;
    };

    /** @brief The interval for updating each of images from camera
     * current fps: 1000/30=33.3
     */
    extern const int FRAME_UPDATE_INTERVAL_MS;


    /** @brief The max time for read an image from camera
	*/
    extern const int MAX_CAPTURE_TIME_MS;

    /** @brief Specify the camera in Vision project
    */
    enum
    {
        LEFT_CAMERA,
        RIGHT_CAMERA,
		MAX_CAMERA_NUMBER
    };


    /** @brief The general size of screen.
     */
    enum {
        SCREEN_HEIGHT = 1080,
        SCREEN_WIDTH = 1920
    };

    /** @brief Specify the camera type
	*/
	enum class CameraType
	{
		NONE = 0,
		MONOCULAR = 1,
		BINOCULAR = 2
	};


    /** @brief Used to store the image in different thread.
	*/
	struct ImageCombo
	{
		cv::Mat image;
        std::chrono::milliseconds cap_time;
		long long tag;
	};


    /** @brief Specify the zoom value when display the image streaming frame.
	*/
	enum class ZoomMode
	{
        NORMAL = 0,         //!< the display view keeps normal
		ZOOM_2X = 1,		//!< the display view will be zoomed 200%
		ZOOM_4X = 2			//!< the display view will be zoomed 400%
	};
}
