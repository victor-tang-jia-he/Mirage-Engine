#include "OpenGL/openGL_shader.h"

void MirageRHI::OpenGLShader::Create(const char* _shaderPath, ShaderType _type)
{
    unsigned int glType = 0;
    if (_type == ShaderType::Vertex)
        glType = GL_VERTEX_SHADER;
    else if (_type == ShaderType::Fragment)
        glType = GL_FRAGMENT_SHADER;
    else
        DEBUG_THROW("Unknown Shader Type Used!");

    CompileShader(_shaderPath, glType, m_Shader);
}

void MirageRHI::OpenGLShader::Destroy()
{
    glDeleteShader(m_Shader);
}

bool MirageRHI::OpenGLShader::CompileShader(const std::string& _path, unsigned int _type, unsigned int& _shaderOut)
{
    std::ifstream file(_path);
    if (!file.is_open())
    {
        DEBUG_THROW("Cant open shader file : %s", _path.c_str());
        return false;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string source = ss.str();
    const char* src = source.c_str();

    _shaderOut = glCreateShader(_type);
    glShaderSource(_shaderOut, 1, &src, nullptr);
    glCompileShader(_shaderOut);

    int success;
    glGetShaderiv(_shaderOut, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(_shaderOut, 512, nullptr, infoLog);
        DEBUG_THROW("Shader compile error : [%s]", infoLog);
        return false;
    }

    DEBUG_LOG("Shader Compiled");
    return true;
}
