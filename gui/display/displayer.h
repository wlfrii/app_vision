#ifndef VIGION_GUI_DISPLAYER_H_LF
#define VIGION_GUI_DISPLAYER_H_LF
#include <cstdint>
#include <QImage>

namespace gui{

class Window;
/**
 * @brief The Displayer class is designed for display image by Qt.
 */
class Displayer
{
public:
    enum DisplayMode{
        DISPLAY_2D,
        DISPLAY_3D
    };
    Displayer(uint16_t win_width, uint16_t win_height, DisplayMode mode);
    ~Displayer();

    void updateImage(const QImage& image, bool is_right = false);

    void show();

    void setDisplayMode(DisplayMode mode);
    void setFullScreen();

    void close();

private:
    Window* _window;
    QImage  _images[2];
    DisplayMode _mode;
};

} // namespace::gui
#endif // VIGION_GUI_DISPLAYER_H_LF
