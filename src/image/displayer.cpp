#include "displayer.h"
#include "../util/util.h"
#include "../util/mtimer.h"

int8_t Displayer::Window::count = -1;

Displayer::Window::Window(uint16_t win_width, uint16_t win_height)
    : width(win_width)
    , height(win_height)
{
    count++;
    name = "DisplayWindow"+std::to_string(count);
}


Displayer::Displayer(uint16_t width, uint16_t height,
              DisplayMode mode/* = DISPLAY_3D*/)
        : _window(Window(width, height))
        , _mode(mode)
        , _save_name("")
        , _save_path("./capture")
        , _is_save_image(false)
{
}

Displayer::~Displayer()
{
    closeWindow();
}

void Displayer::updateImage(const cv::Mat &image, bool is_right/* = false*/)
{
    if(!image.empty()){
        image.copyTo(_images[is_right]);
    }
}

bool Displayer::show()
{
    int width = _images[0].cols;
    int height = _images[0].rows;
    auto fmt = _images[0].channels() == 3 ? CV_8UC3 : CV_8UC4;
    cv::Mat out;
    if(_mode == DISPLAY_2D)
    {
        cv::resize(_images[0], out, cv::Size(_window.width, _window.height));
    }
    else if(_mode == DISPLAY_3D){
        cv::Mat tmp = cv::Mat::zeros(height, width * 2, fmt);
        for(uchar i = 0; i < 2; i++)
        {
            if(!_images[i].empty()){
                _images[i].copyTo(tmp.colRange(i*width, (i + 1)*width));
            }
        }
        cv::resize(tmp, out, cv::Size(_window.width, _window.height));
    }

    if(out.empty()) return false;
    if(_is_save_image){
        doSaveImage();
    }

    cv::imshow(_window.name, out);
    cv::waitKey(10);

    return true;
}

void Displayer::closeWindow()
{
    for(int i = 0; i < _window.count; i++){
        cv::destroyWindow(_window.name);
    }
}

void Displayer::setSavePath(const std::string& path)
{
    _save_path = path;
}

void Displayer::saveImage(const std::string& filename/* = ""*/)
{
    _save_name = filename;
    _is_save_image = true;
}

void Displayer::doSaveImage()
{ 
    std::stringstream ss;
    if(_save_name.empty()){
        ss << util::mtimer::getCurrentTimeStr();
    }
    ss << ".bmp";

    util::MAKE_DIR(_save_path);
    if(_mode == DISPLAY_3D){
        auto left_folder = _save_path + "/left/";
        auto right_folder = _save_path + "/right/";
        util::MAKE_DIR(left_folder);
        util::MAKE_DIR(right_folder);

        auto L_path = left_folder + "L_" + ss.str();
        auto R_path = right_folder + "R_" + ss.str();
        cv::imwrite(L_path, _images[0]);
        cv::imwrite(R_path, _images[1]);

        LOG("Images [L_"+ss.str()+"] and [R_"+ss.str()+"]"
            "has been saved in \""+_save_path+"\".");
    }
    else{
        auto path = _save_path + ss.str();
        cv::imwrite(path, _images[0]);

        LOG("Image ["+ss.str()+"] has been saved in \""+_save_path+"\".");
    }

    _is_save_image = false;
}
