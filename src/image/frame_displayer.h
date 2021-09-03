#ifndef FRAME_DISPLAYER_H_LF
#define FRAME_DISPLAYER_H_LF
#include <array>
#include <string>
#include <vector>
#include "../def/define.h"

#ifdef WITH_QT_GUI
class QLabel;
#endif

enum FrameDisplayWindowType{
    WINDOW_TYPE_NORMAL_STEREO,  //!< The window wil be initialized as 1920 x 540 resolution.
    WINDOW_TYPE_GOOVIS,         //!< For displaying stereo vision in goovis.
    WINDOW_TYPE_COUNT
};

/**
 * @brief This class is designed to display the binocular view.
 * Since there needs only one window to show the frames even there is more
 * than one camera, so we designate this class via a single instance mode.
 */
class FrameDisplayer
{
public:
    FrameDisplayer(const FrameDisplayWindowType& type = WINDOW_TYPE_NORMAL_STEREO);
    ~FrameDisplayer();

    /**
     * @brief The interface called by FrameReader, to update frame.
     * @param cam_id  The id of the camera.
     * @param image  The new frame read by FrameReader.
     */
    void updateFrame(cv::Mat &image, uchar cam_id);

    /**
     * @brief The interface called by Main Thread to show the frame.
     */
    void showFrame();

private:
    void initImshowWindow(int width, int height);
    void saveImage(const cv::Mat &image);

private:
    std::array<cv::Mat, vision::MAX_CAMERA_NUMBER> _images;

    FrameDisplayWindowType _type;

    int         _window_width;
    int         _window_height;
    std::string _window_name;
    static std::vector<std::string> _window_names;

#ifdef WITH_QT_GUI
    QLabel* _display_label;
#endif
};

#endif // FRAME_DISPLAYER_H
