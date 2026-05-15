#include "OpenGL/openGL_pipeline.h"
#include "OpenGL/openGL_shader.h"
#include "OpenGL/openGL_pipeline_parameters.h"
#include <Logs/log.h>

#include <RHI/ubo_structs.h>
#include <i_uniform_buffer.h>


DEFINE_LOG_CATEGORY(LogRHIPipeline)

using namespace MirageEngine::Core::Math;

void MirageRHI::OpenGLPipeline::Create(
    std::shared_ptr<IShader> _vertShader, std::shared_ptr<IShader> _fragShader, std::shared_ptr<IPipelineParameters> _pipelineParameters, std::vector<UBOBlockInfo> _blocks)
{
    m_Parameters = _pipelineParameters;
    m_VertexShader = _vertShader;
    m_FragmentShader = _fragShader;

    m_Program = glCreateProgram();
    glAttachShader(m_Program, m_VertexShader->API_OpenGL().GetGLHandle());
    glAttachShader(m_Program, m_FragmentShader->API_OpenGL().GetGLHandle());
    Link();
    BindUBOBlocks(_blocks);
    LOG(logTemp, LogRHIPipeline, "Pipeline Created with Program ID: ", m_Program);
}

void MirageRHI::OpenGLPipeline::Destroy()
{
    glDeleteProgram(m_Program);

    LOG(logTemp, LogRHIPipeline, "Pipeline Destroyed with Program ID: ", m_Program);
}

bool MirageRHI::OpenGLPipeline::Link() const
{
    glLinkProgram(m_Program);

    GLint success;
    glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint logLength;
        glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetProgramInfoLog(m_Program, logLength, &logLength, log);
        LOG(logError, LogRHIPipeline, "Shader Program linking failed: ", log);
        delete[] log;
        return false;
    }
    if (m_VertexShader->API_OpenGL().GetGLHandle())
    {
        glDetachShader(m_Program, m_VertexShader->API_OpenGL().GetGLHandle());
    }
    if (m_FragmentShader->API_OpenGL().GetGLHandle())
    {
        glDetachShader(m_Program, m_FragmentShader->API_OpenGL().GetGLHandle());
    }
    return true;
}

void MirageRHI::OpenGLPipeline::Bind() const { glUseProgram(m_Program); }

void MirageRHI::OpenGLPipeline::Unbind() const { glUseProgram(0); }

void MirageRHI::OpenGLPipeline::SetInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value); }

void MirageRHI::OpenGLPipeline::SetFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value); }

void MirageRHI::OpenGLPipeline::SetVec3(const std::string& name, const Vector3& value) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    float array[3];
    array[0] = value.X;
    array[1] = value.Y;
    array[2] = value.Z;
    glUniform3fv(location, 1, array);
}

void MirageRHI::OpenGLPipeline::SetVec4(const std::string& name, const Vector4& value) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    float array[4];
    array[0] = value.X;
    array[1] = value.Y;
    array[2] = value.Z;
    array[3] = value.W;
    glUniform4fv(location, 1, array);
}

void MirageRHI::OpenGLPipeline::SetMat3(const std::string& name, const Matrix3& mat) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_TRUE, &mat(0, 0));
}

void MirageRHI::OpenGLPipeline::SetMat4(const std::string& name, const Matrix4& mat) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat(0, 0));
}

void MirageRHI::OpenGLPipeline::SetBool(const std::string& name, bool value) const
{
    GLint location = glGetUniformLocation(m_Program, name.c_str());
    if (location == -1)
    {
        LOG(logWarning, LogRHIPipeline, "Uniform '", name, "' doesn't exist or is not used in Program ID: ", m_Program);
    }
    glUniform1i(location, static_cast<int>(value));
}

void MirageRHI::OpenGLPipeline::ApplyParameters() { m_Parameters->API_OpenGL().ApplyParameters(); }

void MirageRHI::OpenGLPipeline::BindUBOBlocks(std::vector<UBOBlockInfo> _blocks)
{
    for (UBOBlockInfo& block : _blocks)
    {
        GLuint index = glGetUniformBlockIndex(m_Program, block.Name);
        if (index != GL_INVALID_INDEX)
        {
            glUniformBlockBinding(m_Program, index, (int)block.Binding);
            LOG(logTemp, LogRHIPipeline, block.Name, " : ", index, " index used in Program ID: ", m_Program, " bind to ", (int)block.Binding);
        }
        else
        {
            LOG(logWarning, LogRHIPipeline, "Invalid UBO block: ", block.Name, " in program ", m_Program);
        }
    }
}
