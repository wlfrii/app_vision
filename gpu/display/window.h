#ifndef GPU_WINDOW_H_LF
#define GPU_WINDOW_H_LF
#include <cstdint>
#include <string>
#include "../util.h"

struct GLFWwindow;

GPU_NS_BEGIN

class Window
{
public:
    Window(uint16_t width, uint16_t height);
    ~Window();

    /**
     * @brief Initialize GL Window.
     * 
     * @return true 
     * @return false 
     */
    bool initialize();

    /**
     * @brief Activate current window.
     * This is useful when there are more than one GL Window exist.
     */
    void activate();

    /**
     * @brief Swap buffers
     * 
     */
    void refresh();

    bool shouldClose();
    void release();

private:
    void initGLFW();
    bool createGLFWwindow();

    void processKeyboard();

    void toFullScreen();

    GLFWwindow* _window;
    uint16_t    _win_width;
    uint16_t    _win_height;
    std::string _win_name;
};

GPU_NS_END
#endif // GPU_WINDOW_H_LF
