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
#ifdef WITH_QT_GUI
#include <QGuiApplication>
#include <QScreen>
#include <QLabel>
#include <QImage>
#endif

namespace
{
    auto time_point = mtimer::getDurationSinceEpoch();
    size_t fps[2] = {0};
    void checkFPS(int16_t &image_tag, uchar id = 0)
    {
        if(id >= 2) return;
        static std::set<int64_t> frame[2];

        auto now = mtimer::getDurationSinceEpoch();
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



/* ------------------------------------------------------------------------ */
/*                            FrameDisplayWindow                            */
/* ------------------------------------------------------------------------ */

class FrameDisplayWindow
{
protected:
    FrameDisplayWindow()
    {
        prop[WINDOW_TYPE_NORMAL_STEREO] = {960, 270, "Normal Stereo Display"};
        prop[WINDOW_TYPE_GOOVIS] = {1920, 1080, "Goovis Display"};
    }
    ~FrameDisplayWindow() {}
public:
    static FrameDisplayWindow* getInstance()
    {
        static FrameDisplayWindow window;
        return &window;
    }

    void initial(const FrameDisplayWindowType& type,
                 int& win_width, int& win_height, std::string& win_name){
        win_width = prop[type].window_width;
        win_height = prop[type].window_height;
        win_name = prop[type].window_name;
    }

private:
    struct WindowProp
    {
        uint16_t window_width;
        uint16_t window_height;
        std::string window_name;
    };
    WindowProp prop[WINDOW_TYPE_COUNT];
};

/* ------------------------------------------------------------------------ */
/*                              FrameDisplayer                              */
/* ------------------------------------------------------------------------ */

FrameDisplayer::FrameDisplayer(const FrameDisplayWindowType& type/* = WINDOW_TYPE_NORMAL_STEREO*/)
    : _type(type)
    , _display_label(nullptr)
{
    FrameDisplayWindow::getInstance()->initial(type, _window_width, _window_height, _window_name);
}

FrameDisplayer::~FrameDisplayer()
{
    cv::destroyWindow(_window_name);
    DELETE_PIONTER(_display_label);
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
    int height = vision::ImageSize::height;
    // Each image processed by GPU will convert to 4 channel !!
    cv::Mat tmp = cv::Mat::zeros(height, width * 2, CV_8UC4);

    static bool is_start = false;
    if(!_display_label)
    {
        is_start = true;
        initImshowWindow(width, height / 2);
    }

    for(uchar i = 0; i < vision::MAX_CAMERA_NUMBER; i++)
    {
        if(!_images[i].empty()){
            _images[i].copyTo(tmp.colRange(i*width, (i + 1)*width));
        }
    }
    if(!tmp.empty())
    {
        if(CMD::capture.is_take_photo) saveImage(tmp);

        cv::Mat out;
        cv::resize(tmp, out, cv::Size(_window_width, _window_height));

        if(CMD::is_show_fps) ::putFPS(out);
#ifdef WITH_QT_GUI
        QImage qimg((const uchar*)out.data, out.cols, out.rows, out.step, QImage::Format_ARGB32);
        _display_label->setPixmap(QPixmap::fromImage(qimg.copy()));
#else
        cv::imshow(_window_name, out);
        cv::waitKey(10);
#endif
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
    // Note, since OpenCV cannot show fullscreen window on Mac OS, we will display
    // image by Qt when there is a supported Qt.
#ifdef WITH_QT_GUI
    _display_label = new QLabel();
    if(_type == WINDOW_TYPE_NORMAL_STEREO){
        QRect rect = QGuiApplication::screens()[0]->geometry();
        _display_label->move(rect.x(), rect.y());
        _display_label->resize(_window_width, _window_height);
        _display_label->show();
    }
    else if(_type == WINDOW_TYPE_GOOVIS){
        QRect rect = QGuiApplication::screens()[1]->geometry();
        _display_label->move(QPoint(rect.x(), rect.y()));
        _display_label->showFullScreen();
    }
#else
    cv::namedWindow(_window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(_window_name, width, height);
    //        cv::moveWindow(_window_name, 0, -1080);
    //        cv::setWindowProperty(_window_name, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    //        cv::setWindowProperty(_window_name, cv::WND_PROP_AUTOSIZE, cv::WINDOW_AUTOSIZE);
#endif
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
