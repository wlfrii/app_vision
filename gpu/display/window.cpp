#include "window.h"
/** NOTE, GLAD header file should be included before the GLFW header file, since
 * the correct OpenGL header files are included in GLAD
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../util.h"

namespace {
    void frameBufferSizeCallback(GLFWwindow*, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
}

GPU_NS_BEGIN

Window::Window(uint16_t width, uint16_t height)
    : _win_width(width)
    , _win_height(height)
    , _win_name("Displayer")
{

}

Window::~Window()
{
    release();
}

bool Window::initialize()
{
    initGLFW();
    if(!createGLFWwindow()) return false;

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        DEBUG("Failed to initialize GLAD.\n");
        return false;
    }

    return true;
}

void Window::refresh()
{
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void Window::release()
{
    glfwTerminate();
}

// --- PRIVATE ---

void Window::initGLFW()
{
    glfwInit();
    // Configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    /* The following command should be add in Mac OS platform to
       validate the commands above. */
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

bool Window::createGLFWwindow()
{
    _window = glfwCreateWindow(_win_width, _win_height,
                                          _win_name.c_str(), nullptr, nullptr);

    if(_window == nullptr){
        DEBUG("Failed to creat GLFW window.\n");
        glfwTerminate();
        return false;
    }
    else {
        glfwMakeContextCurrent(_window);
        glfwSetFramebufferSizeCallback(_window, ::frameBufferSizeCallback);
    }
    return true;
}

void Window::toFullScreen()
{
    if(_window)
    {
        int monitor_count = 0;
        GLFWmonitor** monitor = glfwGetMonitors(&monitor_count);
        printf("[%d] monitors have been found.\n", monitor_count);
        if(monitor_count >= 2){
            const GLFWvidmode* mode = glfwGetVideoMode(monitor[monitor_count-1]);
            glfwSetWindowMonitor(_window, monitor[monitor_count-1], 0, 0,
                    mode->width, mode->height, mode->refreshRate);
        }
    }
}

GPU_NS_END
