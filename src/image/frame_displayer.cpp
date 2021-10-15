#include "frame_displayer.h"
#include "../util/mtimer.h"
#include "../util/util.h"
#include "../util/define.h"
#include <thread>
#include <sstream>

#ifdef WITH_GL
#include "../gpu/display/displayer.h"
#elif WITH_QT_GUI
#include "../gui/vision_gui.h"
#endif

namespace
{
    auto time_point = util::mtimer::getDurationSinceEpoch();
    size_t fps[2] = {0};
    void checkFPS(int16_t &image_tag, uchar id = 0)
    {
        if(id >= 2) return;
        static std::set<int64_t> frame[2];

        auto now = util::mtimer::getDurationSinceEpoch();
        if ((now - time_point).count() >= 1e6)
        {
            time_point = now;
            fps[id] = frame[id].size();
            frame[id].clear();
            image_tag = 0;
        }
        else
            if (image_tag > 0)
                frame[id].insert(image_tag);
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


FrameDisplayer::FrameDisplayer(uint16_t width, uint16_t height, DisplayMode type)
    : _is_show_fps(true)
{
#ifdef WITH_GL
    //_displayer = new gpu::Displayer(width, height, type);
#elif WITH_QT_GUI
    _displayer = new gui::Displayer(width, height, gui::Displayer::DisplayMode(type));
#else
    _displayer = new Displayer(width, height, Displayer::DisplayMode(type));
#endif
}

FrameDisplayer::~FrameDisplayer()
{
//    if(_displayer){
//        delete  _displayer;
//        _displayer = nullptr;
//    }
}

void FrameDisplayer::updateFrame(cv::Mat &image, uchar cam_id)
{
#if defined WITH_GL || defined WITH_QT_GUI
    cv::Mat tmp;
    cv::cvtColor(image, tmp, cv::COLOR_BGR2RGB);
    //_displayer->updateImage(tmp.ptr<uchar>(0), tmp.cols, tmp.rows,
                            //tmp.channels(), static_cast<bool>(cam_id));
#else
    _displayer->updateFrame(image, cam_id);
#endif
}

void FrameDisplayer::showFrame()
{
    // Because the time interval has been controled in ControlPanel's timer.
    auto start_point = util::mtimer::getCurrentTimePoint();

    bool has_show_correct_image = false;

    if(has_show_correct_image && _is_show_fps)
    {
        //::putFPS(out);
    }

    auto time = util::mtimer::getDurationSince(start_point);
    if(time < vision::FRAME_UPDATE_INTERVAL_MS)
    {
        auto ms = vision::FRAME_UPDATE_INTERVAL_MS - time;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

