#include "frame_displayer.h"
//#include <libutility/timer/mtimer.h>
#include "../def/mtimer.h"
#include "../def/cmd.h"
#include "../def/ptr_define.h"
#include <thread>
#include <sstream>
#ifdef _WIN64
#include <io.h>     ////@func int access(const char *_Filename, int _AccessMode)
#include <direct.h> ////@func int mkdir(const char *_Path), int rmdir(const char *_Path)
#else // (defined __linux__) || (defined __APPLE__)
#include <stdio.h>  ////@func int access(const char *_Filename, int _AccessMode)
#include <unistd.h>
#include <sys/stat.h> ////@func int mkdir(const char *pathname, mode_t mode), int rmdir(const char *_Path)
#endif


namespace
{
    const std::string win_name = "Display Window";
    int win_width = vision::SCREEN_WIDTH;
    int win_height = vision::SCREEN_HEIGHT / 2;

    void init_screen_show(int width = win_width, int height = win_height)
    {
    }


    auto time_point = mtimer::getDurationSinceEpoch();
    void checkFPS(long long image_tag, uchar id)
    {
        static std::set<long long> frame[2];

        auto now = mtimer::getDurationSinceEpoch();
        if ((now - time_point).count() >= 1e6)
        {
            //printf("camera %d ---> fps: %ld\n", id, frame[id].size());
            time_point = now;
            frame[id].clear();
        }
        else
            if (image_tag > 0)
                frame[id].insert(image_tag);
    }

    size_t fps = 0;
    void checkFPS(int16_t &image_tag)
    {
        static std::set<int64_t> frame;

        auto now = mtimer::getDurationSinceEpoch();
        if ((now - time_point).count() >= 1e6)
        {
            //printf("camera ---> fps: %ld\n", frame.size());
            time_point = now;
            fps = frame.size();
            frame.clear();
            image_tag = 0;
        }
        else
            if (image_tag > 0)
                frame.insert(image_tag);
    }

    void putFPS(cv::Mat& image)
    {
        // check the fps
        static int16_t img_count = 0;
        checkFPS(++img_count);
        // put the text on displayed images
        std::stringstream ss;
        ss << ::fps;
        std::string txt = "FPS->" + ss.str();
        cv::putText(image, txt, cv::Point(10, 50)
                    , cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0,0,255,1));
    }
}


FrameDisplayer::FrameDisplayer(std::string window_name/* = Display Window */)
    : _window_width(0)
    , _window_height(0)
    , _window_name(window_name)
{

}

FrameDisplayer::~FrameDisplayer()
{
    cv::destroyWindow(_window_name);
}

void FrameDisplayer::updateFrame(cv::Mat &image, uchar cam_id)
{
    if (cam_id < vision::MAX_CAMERA_NUMBER)
    {
        if(image.type() == CV_8UC3){
            cv::Mat temp;
            cv::cvtColor(image, temp, cv::COLOR_BGR2BGRA);
            temp.copyTo(_images[cam_id]);
        }
        else if(image.type() == CV_8UC4){
            image.copyTo(_images[cam_id]);
        }
        else{
            LOG("The displayed image should be converted into CV_8UC4");
        }
    }
}

void FrameDisplayer::showFrame()
{
    // Because the time interval has been controled in ControlPanel's timer.
    auto start_point = mtimer::getCurrentTimePoint();

    int width = vision::ImageSize::width;
    // Each image processed by GPU will convert to 4 channel !!
    cv::Mat tmp(vision::ImageSize::height, width * 2, CV_8UC4);

    static bool is_start = false;
    if(!is_start)
    {
        is_start = true;
        init_screen_show();
    }

    for(uchar i = 0; i < vision::MAX_CAMERA_NUMBER; i++)
    {
        if(!_images[i].empty()){
            _images[i].copyTo(tmp.colRange(i*width, (i + 1)*width));
        }
    }
    if(!tmp.empty())
    {
        if(CMD::capture.is_take_photo){
            saveImage(tmp);
        }

        cv::Mat out;
        cv::resize(tmp, out, cv::Size(win_width, win_height));   

        if(CMD::is_show_fps){
            ::putFPS(out);
        }

        cv::imshow(win_name, out);
        cv::waitKey(10);
    }

    auto time = mtimer::getDurationSince(start_point);
    if(time < vision::FRAME_UPDATE_INTERVAL_MS)
    {
        auto ms = vision::FRAME_UPDATE_INTERVAL_MS - time;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

void FrameDisplayer::initImshowWindow(int width, int height)
{
    cv::namedWindow(_window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(_window_name, width, height);
    cv::moveWindow(_window_name, 0, 0);

    //cv::setWindowProperty(window_name, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    //cv::setWindowProperty(win_name, cv::WND_PROP_AUTOSIZE, cv::WINDOW_AUTOSIZE);
}

/* Make a dir if the dir is not exist*/
#if (defined _WIN64)
#define MAKE_DIR(folder) { \
    if(_access(folder.c_str(), 0) == -1){ \
        _mkdir(folder.c_str()); }}
#else // (defined __linux__) || (defined __APPLE__)
#define MAKE_DIR(folder) { \
    if(access(folder.c_str(), 0) == -1){ \
        mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); }}
#endif

void FrameDisplayer::saveImage(const cv::Mat &image)
{
    auto left_folder = CMD::capture.save_path + "/left/";
    auto right_folder = CMD::capture.save_path + "/right/";
    MAKE_DIR(CMD::capture.save_path);
    MAKE_DIR(left_folder);
    MAKE_DIR(right_folder);

    std::stringstream ss;
    if(CMD::capture.save_name.empty()){
        ss << mtimer::getCurrentTimeStr();
    }
    static int count = 1;
	if (CMD::capture.save_num > 1) {
        ss << "_";
        ss << count;
	}
    ss << ".bmp";

    int cols = image.cols;
    auto L_path = left_folder + "L_" + ss.str();
    auto R_path = right_folder + "R_" + ss.str();
    cv::imwrite(L_path, image.colRange(1, cols/2));
    cv::imwrite(R_path, image.colRange(cols/2, cols));

	if (count >= CMD::capture.save_num) {
		CMD::capture.is_take_photo = false;
		count = 1;
		CMD::capture.finished = true;
	}
	else {
		count++;
	}
}
