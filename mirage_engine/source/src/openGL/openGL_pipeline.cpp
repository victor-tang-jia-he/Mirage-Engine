#include "openGL/openGL_pipeline.h"

void MirageRHI::OpenGLPipeline::Create(const char* vertPath, const char* fragPath)
{
    SetVertexShader(vertPath);
    SetFragmentShader(fragPath);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glUseProgram(program);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    Link();
}

void MirageRHI::OpenGLPipeline::Destroy()
{
    glDeleteProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteVertexArrays(1, &vao);
}

bool MirageRHI::OpenGLPipeline::SetVertexShader(const std::string& path)
{
    return CompileShader(path, GL_VERTEX_SHADER, vertexShader);
}

bool MirageRHI::OpenGLPipeline::SetFragmentShader(const std::string& path)
{
    return CompileShader(path, GL_FRAGMENT_SHADER, fragmentShader);
}

bool MirageRHI::OpenGLPipeline::Link()
{
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetProgramInfoLog(program, logLength, &logLength, log);
        std::cerr << "Shader Program linking failed: " << log << "\n";
        delete[] log;
        return false;
    }
    if (vertexShader) {
        glDetachShader(program, vertexShader);
        glDeleteShader(vertexShader);
    }
    if (fragmentShader) {
        glDetachShader(program, fragmentShader);
        glDeleteShader(fragmentShader);
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
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void MirageRHI::OpenGLPipeline::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void MirageRHI::OpenGLPipeline::SetVec3(const std::string& name, const Vector3& value) const
{
    int location = glGetUniformLocation(program, name.c_str());
    float array[3];
    array[0] = value.X;
    array[1] = value.Y;
    array[2] = value.Z;
    glUniform3fv(location, 1, array);
}

void MirageRHI::OpenGLPipeline::SetMat3(const std::string& name, const Matrix3& mat) const
{
    int location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_TRUE, &mat.M[0]);
}

void MirageRHI::OpenGLPipeline::SetMat4(const std::string& name, const Matrix4& mat) const
{
    int location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat.M[0]);
}

void MirageRHI::OpenGLPipeline::SetBool(const std::string& name, bool value) const
{
    GLint location = glGetUniformLocation(program, name.c_str());
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

    glAttachShader(program, shaderOut);
    DEBUG_LOG("Shader Compiled\n");
    return true;
}
