#include "camera.h"
#include "camera_parameters.h"
#include "../image/frame_displayer.h"
#include "../image/frame_reader.h"
#include "../image/image_processor.h"
#include "../util/util.h"
#include "../util/mtimer.h"
#include "../util/define.h"

Camera::Camera(uchar cam_id, uchar usb_id)
    : _id(cam_id)
    , _frame_reader(new FrameReader(usb_id, cam_id))
{
    init();
}

Camera::Camera(uchar cam_id, const std::string &video_path)
    : _id(cam_id)
    , _frame_reader(new FrameReader(video_path))
{
    init();
}

Camera::~Camera()
{
    DELETE_PIONTER(_frame_reader);
    DELETE_PIONTER(_frame_displayer);
}

void Camera::init()
{
    _thread = std::thread(&Camera::run, this);
    _thread.detach();
    _frame_displayer = new FrameDisplayer(
                vision::ImageSize::width, vision::ImageSize::height);
}

void Camera::run()
{
    cv::Mat frame;
    cv::Mat res_frame;
    while(true)
    {
        MTIMER__GET_CURRENT_TIME_POINT(start_time_point);

        // get frame
        if (_frame_reader->getFrame(frame))
		{
            ImageProcessor::getInstance()->processImage(frame, res_frame, _id);

            _frame_displayer->updateFrame(res_frame, _id);
		}

        // check time
        auto time = util::mtimer::getDurationSince(start_time_point);
        if(time < vision::FRAME_UPDATE_INTERVAL_MS)
        {
            auto ms = vision::FRAME_UPDATE_INTERVAL_MS - time;
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }
    }
}
