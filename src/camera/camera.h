#ifndef CAMERA_H
#define CAMERA_H
#include <thread>
#include <opencv2/opencv.hpp>

class FrameReader;
class FrameDisplayer;

class Camera
{
public:
    Camera(uchar usb_id, uchar cam_id);

    ~Camera();

private:
    void run [[noreturn]] ();


    uchar           _id;
    std::thread     _thread;

    FrameReader*    _frame_reader;
    FrameDisplayer* _frame_displayer;
};

#endif // CAMERA_H
