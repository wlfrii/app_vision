#include "frame_reader.h"
//#include <libutility/timer/mtimer.h>
#include "../def/mtimer.h"
#include "../def/ptr_define.h"
#include "../def/define.h"
#if LINUX
#include "unix/v4l2_capture.h"
#endif

namespace
{
    long long img_tag = 0;

    long long getImageTag() { img_tag++; return img_tag; }
}


// #ifdef __linux__
FrameReader::FrameReader(uchar usb_id, uchar cam_id, uint image_width, uint image_height)
    : _usb_id(usb_id)
    , _cam_id(cam_id)
    , _image_width(image_width)
    , _image_height(image_height)
    , _sharpness(3)
{
    // capture = new V4L2Capture(image_width, image_height, 3);
    // capture->openDevice(usb_id);
    //m_thread = std::thread(&FrameReader::runFrameStreaming, this);
    //m_thread.detach();
}

FrameReader::FrameReader(const std::string& video_path)
    : _usb_id(0)
    , _cam_id(0)
    , _image_width(0)
    , _image_height(0)
    , _sharpness(3)
{
    _video_capture.open(video_path);
    if (!_video_capture.isOpened()) {
        LOG("Cannot open the specified video!");
        system("pause");
        exit(-1);
    }
}


FrameReader::~FrameReader()
{
#ifdef __linux__
    DELETE_PIONTER(capture);
#endif
    _video_capture.release();
}


bool FrameReader::getFrame(cv::Mat& frame)
{
#ifdef __linux__
	auto start_time_point = mtimer::getDurationSinceEpoch();

	bool flag = capture->ioctlDequeueBuffers(frame);
	
	float time = mtimer::getDurationSince(start_time_point);
	DEBUG_TIME("Camera id: %d, read image: %f ms.\n", cam_id, time);

	flag = flag && (time <= vision::MAX_CAPTURE_TIME_MS) && (!frame.empty());
	if (flag == false)
	{
		LOG("camera ID: %d, USB ID: %d, time: %f ms, empty: %d.\n",
			cam_id, usb_id, time, frame.empty());

		while (!capture->openDevice(usb_id))
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			printf("Camera %d is retrying to connection!!!\n", cam_id);
		}
	}
	else
		return true;
#endif
    _video_capture.read(frame);
	if (!frame.empty()) {
		return true;
	}

    return false;
}
