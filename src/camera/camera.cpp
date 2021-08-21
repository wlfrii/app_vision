#include "camera.h"
#include "camera_parameters.h"
#include "../image/frame_displayer.h"
#include "../image/frame_reader.h"
#include "../image/image_processor.h"
#include "../def/ptr_define.h"
//#include <libutility/timer/mtimer.h>
#include "../def/mtimer.h"

Camera::Camera(uchar usb_id, uchar cam_id, uint image_width, uint image_height)
    : _id(cam_id)
    , _frame_reader(new FrameReader(usb_id, cam_id, image_width, image_height))
{
    _thread = std::thread(&Camera::run, this);
    _thread.detach();
    //_frame_displayer = FrameDisplayer::createInstance();
}

Camera::~Camera()
{
    DELETE_PIONTER(_frame_reader);
}

void Camera::run()
{
    cv::Mat frame;
    cv::Mat res_frame;
    while(true)
    {
        auto start_time_point = mtimer::getCurrentTimePoint();

        // get frame
        if (_frame_reader->getFrame(frame))
		{
#ifdef __DEBUG_TIME
			start_time_point = mtimer::getDurationSinceEpoch();
#endif
            ImageProcessor::getInstance()->processImage(frame, res_frame, _id);
#ifdef __DEBUG_TIME
			time = mtimer::getDurationSince(start_time_point);
			printf("Camera id: %d, update frame time: %f ms\n", m_cam_id, time);
#endif

            _frame_displayer->updateFrame(res_frame, _id);
		}
#ifdef _WIN64
		else {
			LOG("The video has been processed done.\n");
		}
#endif

        // check time
        auto time = mtimer::getDurationSince(start_time_point);
        if(time < vision::FRAME_UPDATE_INTERVAL_MS)
        {
            auto ms = vision::FRAME_UPDATE_INTERVAL_MS - time;
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }
    }
}
