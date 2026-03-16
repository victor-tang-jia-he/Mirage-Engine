#include "openGL/openGL_pipeline.h"

void MirageRHI::OpenGLPipeline::Create(const char* vertPath, const char* fragPath)
{
    SetVertexShader(vertPath);
    SetFragmentShader(fragPath);

    m_Program = glCreateProgram();
    glAttachShader(m_Program, m_VertexShader);
    glAttachShader(m_Program, m_FragmentShader);
    glLinkProgram(m_Program);
    glUseProgram(m_Program);

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    Link();
}

void MirageRHI::OpenGLPipeline::Destroy()
{
    glDeleteProgram(m_Program);
    glDeleteShader(m_VertexShader);
    glDeleteShader(m_FragmentShader);
    glDeleteVertexArrays(1, &m_VAO);
}

bool MirageRHI::OpenGLPipeline::SetVertexShader(const std::string& path)
{
    return CompileShader(path, GL_VERTEX_SHADER, m_VertexShader);
}

bool MirageRHI::OpenGLPipeline::SetFragmentShader(const std::string& path)
{
    return CompileShader(path, GL_FRAGMENT_SHADER, m_FragmentShader);
}

bool MirageRHI::OpenGLPipeline::Link()
{
    glLinkProgram(m_Program);

    GLint success;
    glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetProgramInfoLog(m_Program, logLength, &logLength, log);
        std::cerr << "Shader Program linking failed: " << log << "\n";
        delete[] log;
        return false;
    }
    if (m_VertexShader) {
        glDetachShader(m_Program, m_VertexShader);
        glDeleteShader(m_VertexShader);
    }
    if (m_FragmentShader) {
        glDetachShader(m_Program, m_FragmentShader);
        glDeleteShader(m_FragmentShader);
    }
    return true;
}

void MirageRHI::OpenGLPipeline::SetLayout()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
}

void MirageRHI::OpenGLPipeline::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value);
}

void MirageRHI::OpenGLPipeline::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
}

void MirageRHI::OpenGLPipeline::SetVec3(const std::string& name, const Vector3& value) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    float array[3];
    array[0] = value.x;
    array[1] = value.y;
    array[2] = value.z;
    glUniform3fv(location, 1, array);
}

void MirageRHI::OpenGLPipeline::SetMat3(const std::string& name, const Matrix3& mat) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_TRUE, mat.m[0]);
}

void MirageRHI::OpenGLPipeline::SetMat4(const std::string& name, const Matrix4& mat) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, mat.m[0]);
}

void MirageRHI::OpenGLPipeline::SetBool(const std::string& name, bool value) const
{
    GLint location = glGetUniformLocation(m_Program, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist or is not used.\n";
    }
    glUniform1i(location, static_cast<int>(value));
}

bool MirageRHI::OpenGLPipeline::CompileShader(const std::string& path, unsigned int type, unsigned int& shaderOut)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        DEBUG_THROW("Cant open shader file : %s\n", path.c_str());
        return false;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string source = ss.str();
    const char* src = source.c_str();

    shaderOut = glCreateShader(type);
    glShaderSource(shaderOut, 1, &src, nullptr);
    glCompileShader(shaderOut);

    int success;
    glGetShaderiv(shaderOut, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderOut, 512, nullptr, infoLog);
        DEBUG_THROW("Shader compile error : [%c]\n", infoLog);
        return false;
    }

    glAttachShader(m_Program, shaderOut);
    DEBUG_LOG("Shader Compiled\n");
    return true;
}
