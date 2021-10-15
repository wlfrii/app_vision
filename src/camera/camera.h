#ifndef CAMERA_H_LF
#define CAMERA_H_LF
#include <thread>
#include <opencv2/opencv.hpp>

class FrameReader;
class FrameDisplayer;

class Camera
{
public:
    Camera(uchar cam_id, uchar usb_id);
    Camera(uchar cam_id, const std::string& video_path);

    ~Camera();

private:
    void init();

    void run [[noreturn]] ();


    uchar           _id;
    std::thread     _thread;

    FrameReader*    _frame_reader;
    FrameDisplayer* _frame_displayer;
};

#endif // CAMERA_H_LF
