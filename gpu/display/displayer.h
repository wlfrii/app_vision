#ifndef GPU_DISPLAYER_H_LF
#define GPU_DISPLAYER_H_LF
#include <cstdint>
#include "../util.h"
GPU_NS_BEGIN

class Window;
class Shader;
struct VAVBEBO;

class Displayer
{
public:
    enum DisplayMode{
        DISPLAY_2D,
        DISPLAY_3D
    };
    Displayer(uint16_t width, uint16_t height, DisplayMode mode = DISPLAY_3D);
    Displayer(const Displayer &) = delete;
    ~Displayer();

    bool initialize();

    void updateImage(uint8_t* data, uint16_t im_w, uint16_t im_h, uint8_t im_c,
                     bool is_right);

    void render();

private:
    uint16_t    _width;
    uint16_t    _height;

    Window*     _window;
    Shader*     _im_shader;
    VAVBEBO*    _im_vavbebo;

    DisplayMode _mode;
    unsigned int _texture[2];

    bool        _has_init;
};

GPU_NS_END
#endif // GPU_DISPLAYER_H_LF
