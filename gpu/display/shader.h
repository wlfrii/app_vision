#ifndef GPU_SHADER_H_LF
#define GPU_SHADER_H_LF
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "../util.h"
GPU_NS_BEGIN

class Shader
{
public:
    Shader() {}

    void load(const char* vs_path, const char* fs_path);

    // activate the shader
    void use()
    {
        glUseProgram(ID);
    }

    // utility uniform functions
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setMat4f(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setVec3f(const std::string &name, const glm::vec3 &vec) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
    }

    void release()
    {
        glDeleteProgram(ID);
    }

    unsigned int ID;
};


GPU_NS_END
#endif // GPU_SHADER_H_LF
