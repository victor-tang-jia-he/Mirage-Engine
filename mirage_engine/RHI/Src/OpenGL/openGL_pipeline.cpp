#include "OpenGL/openGL_pipeline.h"
#include "OpenGL/openGL_shader.h"

void MirageRHI::OpenGLPipeline::Create(IShader* _vertShader, IShader* _fragShader)
{
    m_VertexShader = _vertShader;
    m_FragmentShader = _fragShader;

    m_Program = glCreateProgram();
    glAttachShader(m_Program, m_VertexShader->API_OpenGL().GetGLHandle());
    glAttachShader(m_Program, m_FragmentShader->API_OpenGL().GetGLHandle());
    glLinkProgram(m_Program);
    glUseProgram(m_Program);
}

void MirageRHI::OpenGLPipeline::Destroy()
{
    glDeleteProgram(m_Program);
}

bool MirageRHI::OpenGLPipeline::Link()
{
    return false;
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

// Struct
void MirageRHI::PipelineParamaters::OpenFile()
{
}

void MirageRHI::PipelineParamaters::ResetFile()
{
}

std::string MirageRHI::PipelineParamaters::Print()
{
    return "Print";
}

std::string MirageRHI::PipelineParamaters::Throw()
{
    return "Throw";
}