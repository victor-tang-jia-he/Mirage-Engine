#include <OpenGL/openGL_shader.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogRHIShader)

bool MirageRHI::OpenGLShader::Create(const char* _shaderPath, ShaderType _type)
{
    m_Type = _type;
    unsigned int glType = 0;
    if (_type == ShaderType::VERTEX)
        glType = GL_VERTEX_SHADER;
    else if (_type == ShaderType::FRAGMENT)
        glType = GL_FRAGMENT_SHADER;
    else
        LOG(logError, LogRHIShader, "Unknown Shader Type Used!");
    if (!CompileShader(_shaderPath, glType, m_Shader))
    {
        return false;
    }

    LOG(logTemp, LogRHIShader, "Shader Created with ID: ", m_Shader);
    return true;
}

void MirageRHI::OpenGLShader::Destroy()
{
    glDeleteShader(m_Shader);

    LOG(logTemp, LogRHIShader, "Shader Destroyed with ID: ", m_Shader);
}

bool MirageRHI::OpenGLShader::CompileShader(const std::string& _path, unsigned int _type, unsigned int& _shaderOut)
{
    std::ifstream file(_path);
    if (!file.is_open())
    {
        LOG(logError, LogRHIShader, "Can't open shader file: ", _path);
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
        LOG(logError, LogRHIShader, "Shader compile error: ", infoLog);
        return false;
    }

    LOG(logTemp, LogRHIShader, "Shader Compiled with ID: ", _shaderOut);
    return true;
}
