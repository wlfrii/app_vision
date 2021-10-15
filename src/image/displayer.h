#ifndef VISION_CV_DISPLAYER_H_LF
#define VISION_CV_DISPLAYER_H_LF
#include <opencv2/opencv.hpp>

class Displayer
{
    class Window
    {
    public:
        Window(uint16_t win_width, uint16_t win_height);
        ~Window() {}

        uint16_t    width;
        uint16_t    height;
        std::string name;

        static int8_t count;
    };
public:
    using DisplayMode = unsigned char;
    static const DisplayMode DISPLAY_2D = 0;
    static const DisplayMode DISPLAY_3D = 1;

    Displayer(uint16_t width, uint16_t height,
              DisplayMode mode = DISPLAY_3D);

    Displayer(const Displayer &) = delete;
    ~Displayer();

    void updateImage(const cv::Mat &image, bool is_right = false);

    bool show();

    void closeWindow();

    void setSavePath(const std::string& path);

    void saveImage(const std::string& filename = "");

private:
    void doSaveImage();

    cv::Mat _images[2];
    Window  _window;
    bool    _mode;

    std::string _save_name;
    std::string _save_path;
    bool        _is_save_image;
};

#endif // VISION_CV_DISPLAYER_H_LF
