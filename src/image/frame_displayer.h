/**
 * To display the image, OpenGL is the prefered tool since its efficiency.
 * Thus, This module has change the display method from OpenCV to OpenGL,
 * and GLFW and GLAD is required in this project.
 */
#ifndef FRAME_DISPLAYER_H_LF
#define FRAME_DISPLAYER_H_LF
#include <string>
#include <opencv2/opencv.hpp>

#ifdef WITH_GL
namespace gpu{ class Displayer; }
#elif WITH_QT_GUI
namespace gui{ class Displayer; }
#else
class Displayer;
#endif

/**
 * @brief This class is designed to display the binocular view.
 * Since there needs only one window to show the frames even there is more
 * than one camera, so we designate this class via a single instance mode.
 */
class FrameDisplayer
{
public:
    enum DisplayMode{
        DISPLAY_2D = 0,
        DISPLAY_3D = 1
    };

    FrameDisplayer(uint16_t width, uint16_t height, DisplayMode type = DISPLAY_3D);
    ~FrameDisplayer();

    /**
     * @brief The interface called by FrameReader, to update frame.
     * @param cam_id  The id of the camera. If Display_2D mode is set, the
     * cam_id is not required, and is useless even is set.
     * @param image  The new frame read by FrameReader.
     */
    void updateFrame(cv::Mat &image, uchar cam_id = 0);

    /**
     * @brief The interface called by Main Thread to show the frame.
     */
    void showFrame();

private:
    void saveImage(const cv::Mat &image);

#ifdef WITH_GL
    gpu::Displayer* _displayer;
#elif WITH_QT_GUI
    gui::Displayer* _displayer;
#else
    Displayer*      _displayer;
#endif

    bool _is_show_fps;
};

#endif // FRAME_DISPLAYER_H
