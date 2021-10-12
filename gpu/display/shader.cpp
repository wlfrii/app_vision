#include "shader.h"
#include <fstream>
#include <sstream>


namespace {

void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[512];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            DEBUG("ERROR::SHADER_COMPILATION_ERROR of type: %s\n\t%s\n",
                type.c_str(), infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            DEBUG("ERROR::PROGRAM_LINKING_ERROR of type: %s\n\t%s\n",
                type.c_str(), infoLog);
        }
    }
}

}

GPU_NS_BEGIN

void Shader::load(const char *vs_path, const char *fs_path)
{
    /* 1. retrieve the vertex/fragment source code from filePath */
    std::string vertex_code, fragment_code;
    std::ifstream vs_file, fs_file;
    // ensure ifstream objects can throw exceptions:
    vs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vs_file.open(vs_path);
        fs_file.open(fs_path);
        std::stringstream vs_stream, fs_stream;
        // read file's buffer contents into streams
        vs_stream << vs_file.rdbuf();
        fs_stream << fs_file.rdbuf();
        // close file handlers
        vs_file.close();
        fs_file.close();
        // convert stream into string
        vertex_code   = vs_stream.str();
        fragment_code = fs_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        DEBUG("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
    }
    const char* vs_code = vertex_code.c_str();
    const char* fs_code = fragment_code.c_str();

    /* 2. compile shaders */
    unsigned int vertex_shader, fragment_shader;
    // vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_code, NULL);
    glCompileShader(vertex_shader);
    ::checkCompileErrors(vertex_shader, "VERTEX");
    // fragment Shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_code, NULL);
    glCompileShader(fragment_shader);
    ::checkCompileErrors(fragment_shader, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    glLinkProgram(ID);
    ::checkCompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

GPU_NS_END
