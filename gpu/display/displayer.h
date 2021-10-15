#ifndef GPU_DISPLAYER_H_LF
#define GPU_DISPLAYER_H_LF
#include <cstdint>
#include <string>

namespace gpu{

class Window;
class Shader;
struct VAVBEBO;

class Displayer
{
public:
    using DisplayMode = bool;
    static constexpr DisplayMode DISPLAY_2D = 0;
    static constexpr DisplayMode DISPLAY_3D = 1;
    Displayer(uint16_t width, uint16_t height, DisplayMode mode = DISPLAY_3D);
    Displayer(const Displayer &) = delete;
    ~Displayer();

    /**
     * @brief Initialize displayer for showing image.
     * 
     * @return true 
     * @return false 
     */
    bool initialize();

    /**
     * @brief Update image buffer.
     * 
     * @param data The image data.
     * @param im_w The width of the image.
     * @param im_h The height of the image.
     * @param im_c The channels of the image.
     * @param is_right To specify left or right image.
     */
    void updateImage(uint8_t* data, uint16_t im_w, uint16_t im_h, uint8_t im_c,
                     bool is_right = false);

    /**
     * @brief Render the input image data.
     * 
     */
    void render();

    /**
     * @brief Return the window close-status.
     * This is controled by 'ESCAPE' button.
     * 
     * @return true 
     * @return false 
     */
    bool shouldClose();

    void setSavePath(const std::string& path);
    void saveTexture(const std::string& filename = "");

private:
    uint16_t    _width;
    uint16_t    _height;

    Window*     _window;
    Shader*     _im_shader;
    VAVBEBO*    _im_vavbebo;

    DisplayMode _mode;
    unsigned int _texture[2];

    bool        _has_init;

    // For save texture
    std::string _save_name;
    std::string _save_path;
};

} // namespace::gpu
#endif // GPU_DISPLAYER_H_LF
