#ifndef GPU_VAVBEBO_H_LF
#define GPU_VAVBEBO_H_LF
#include <glad/glad.h>
#include "../util.h"
GPU_NS_BEGIN

struct VAVBEBO
{
public:
    VAVBEBO() : _vao(0), _vbo(0), _ebo(0), _is_bind(false) {}
    ~VAVBEBO() { release(); }

    void bind(float* vertices, size_t vertices_size,
              unsigned int* indices = nullptr, size_t indices_size = 0)
    {
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

        if(indices){
            glGenBuffers(1, &_ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
        }
        _is_bind = true;
    }

    void bindVertexArray()
    {
        glBindVertexArray(_vao);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    void release()
    {
        if(_is_bind){
            glDeleteVertexArrays(1, &_vao);
            glDeleteBuffers(1, &_vbo);
            glDeleteBuffers(1, &_ebo);
            _is_bind = false;
        }
    }

private:
    GLuint _vao;
    GLuint _vbo;
    GLuint _ebo;
    bool _is_bind;
};

GPU_NS_END
#endif // GPU_VAVBEBO_H_LF
