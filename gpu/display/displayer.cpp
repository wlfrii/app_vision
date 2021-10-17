#include "displayer.h"
#include "window.h"
#include "shader.h"
#include "vavbebo.h"
#include <sstream>

GPU_NS_BEGIN

Displayer::Displayer(uint16_t width, uint16_t height, DisplayMode mode)
    : _width(width), _height(height)
    , _window(nullptr)
    , _im_shader(nullptr)
    , _im_vavbebo(nullptr)
    , _mode(mode)
    , _has_init(false)
    , _save_name("")
    , _save_path("./capture")
{
    _texture[0] = 0;
    _texture[1] = 0;
    DEBUG("Display mode %d\n", _mode);
}


Displayer::~Displayer()
{
    DELETE_POINT(_window);
    DELETE_POINT(_im_shader);
    DELETE_POINT(_im_vavbebo);
}


bool Displayer::initialize()
{
    _window = new Window(960, 540);
    if(!_window->initialize()) return false;

    _im_shader = new Shader();
    _im_shader->load("imtexture.vs", "imtexture.fs");

    float vertices[] = {
        // positions         // texture coords
        1.0f,  1.0f, 1.0f,   1.0f, 0.0f, // top right
        1.0f, -1.0f, 1.0f,   1.0f, 1.0f, // bottom right
        -1.0f, -1.0f, 1.0f,   0.0f, 1.0f, // bottom left
        -1.0f,  1.0f, 1.0f,   0.0f, 0.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    _im_vavbebo = new VAVBEBO();
    _im_vavbebo->bind(vertices, sizeof(vertices), indices, sizeof(indices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    _has_init = true;
    return _has_init;
}


void Displayer::updateImage(
        uint8_t *data, uint16_t im_w, uint16_t im_h, uint8_t im_c, bool is_right)
{
    if(!_has_init){
        DEBUG("The dispalyer was not initialized\n");
        return;
    }
    if(!data){
        DEBUG("Empty image.\n");
        return;
    }

    glGenTextures(1, &_texture[is_right]);
    glBindTexture(GL_TEXTURE_2D, _texture[is_right]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum fmt = GL_RGB;
    if(im_c == 4) fmt = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, fmt, im_w, im_h, 0, fmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}


void Displayer::render()
{
    if(!_has_init){
        DEBUG("The dispalyer was not initialized\n");
        return;
    }
    _window->activate();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if(_mode == DISPLAY_3D){
        glViewport(0, 0, _width, _height*2);
    }
    _im_shader->use();
    _im_vavbebo->bindVertexArray();
    glBindTexture(GL_TEXTURE_2D, _texture[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    if(_mode == DISPLAY_3D && _texture[1] != 0){
        glViewport(_width, 0, _width, _height*2);
        _im_shader->use();
        _im_vavbebo->bindVertexArray();
        glBindTexture(GL_TEXTURE_2D, _texture[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    _window->refresh();
}


bool Displayer::shouldClose()
{
    return _window->shouldClose();
}


void Displayer::setSavePath(const std::string& path)
{
    _save_path = path;
}


void Displayer::saveTexture(const std::string& filename/* = ""*/)
{
    _save_name = filename;
    // To be finished
    std::stringstream ss;
    if(_save_name.empty()){
        ss << util::getCurrentTimeStr();
    }
    ss << ".bmp";
    std::string name = ss.str();

    util::MAKE_DIR(_save_path);
    if(_mode == DISPLAY_3D){
        auto left_folder = _save_path + "/left/";
        auto right_folder = _save_path + "/right/";
        util::MAKE_DIR(left_folder);
        util::MAKE_DIR(right_folder);

        auto L_path = left_folder + "L_" + name;
        auto R_path = right_folder + "R_" + name;
        //cv::imwrite(L_path, _images[0]);
        //cv::imwrite(R_path, _images[1]);

        LOG("Images [L_%s] and [R_%s] has been saved in \"%s\".",
            name.c_str(), name.c_str(), _save_path.c_str());
    }
    else{
        auto path = _save_path + ss.str();
        //cv::imwrite(path, _images[0]);

        LOG("Image [%s] has been saved in \"%s\".", name.c_str(), _save_path.c_str());
    }
}

GPU_NS_END
