#include "OpenGL/openGL_pipeline.h"
#include "OpenGL/openGL_shader.h"
#include "OpenGL/openGL_pipeline_parameters.h"

void MirageRHI::OpenGLPipeline::Create(IShader* _vertShader, IShader* _fragShader, IPipelineParameters* _pipelineParameters)
{
    m_Parameters = _pipelineParameters;
    m_VertexShader = _vertShader;
    m_FragmentShader = _fragShader;

    m_Program = glCreateProgram();
    glAttachShader(m_Program, m_VertexShader->API_OpenGL().GetGLHandle());
    glAttachShader(m_Program, m_FragmentShader->API_OpenGL().GetGLHandle());
    Link();
}

void MirageRHI::OpenGLPipeline::Destroy()
{
    glDeleteProgram(m_Program);
}

bool MirageRHI::OpenGLPipeline::Link() const
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
    if (m_VertexShader->API_OpenGL().GetGLHandle()) {
        glDetachShader(m_Program, m_VertexShader->API_OpenGL().GetGLHandle());
        glDeleteShader(m_VertexShader->API_OpenGL().GetGLHandle());
    }
    if (m_FragmentShader->API_OpenGL().GetGLHandle()) {
        glDetachShader(m_Program, m_FragmentShader->API_OpenGL().GetGLHandle());
        glDeleteShader(m_FragmentShader->API_OpenGL().GetGLHandle());
    }
    return true;
}

void MirageRHI::OpenGLPipeline::Bind() const
{
    glUseProgram(m_Program);
}

void MirageRHI::OpenGLPipeline::Unbind() const
{
    glUseProgram(0);
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
    if (location == -1)
    {
		DEBUG_THROW("Warning: uniform '%s' doesn't exist or is not used.", name.c_str());
    }
    glUniform1i(location, static_cast<int>(value));
}

void MirageRHI::OpenGLPipeline::ApplyParameters()
{
    m_Parameters->API_OpenGL().ApplyParameters();
}