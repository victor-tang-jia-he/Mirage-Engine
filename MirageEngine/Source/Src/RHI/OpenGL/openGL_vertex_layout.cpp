#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include <OpenGL/openGL_vertex_layout.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogRHIVertexLayout)

void MirageRHI::OpenGLVertexLayout::Create()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    LOG(logTemp, LogRHIVertexLayout, "Vertex Layout Created with VAO ID: ", m_VAO);
}

void MirageRHI::OpenGLVertexLayout::Destroy()
{
    ClearAttributes();
    glDeleteVertexArrays(1, &m_VAO);

    LOG(logTemp, LogRHIVertexLayout, "Vertex Layout Destroyed with VAO ID: ", m_VAO);
}

void MirageRHI::OpenGLVertexLayout::AddAttribute(std::shared_ptr<VertexAttribute> _newAttribute, uint32_t _index)
{
    if (m_Attributes.contains(_index))
    {
        LOG(logTemp, LogRHIVertexLayout, "Attribute index already used in this layout. Old ", m_Attributes.at(_index)->Name, "attribute replaced by ", _newAttribute->Name);
        RemoveAttribute(_index);
    }
    m_Attributes.emplace(_index, _newAttribute);
}

void MirageRHI::OpenGLVertexLayout::RemoveAttribute(uint32_t _targetAttributeIndex)
{
    if (m_Attributes.contains(_targetAttributeIndex))
    {
        m_Attributes.erase(_targetAttributeIndex);
        return;
    }
    LOG(logWarning, LogRHIVertexLayout, "Attribute index not used in this layout!");
}

void MirageRHI::OpenGLVertexLayout::RemoveAttribute(std::string _targetAttributeName)
{
    for (uint32_t index = 0; index < m_Attributes.size(); ++index)
    {
        if (m_Attributes.contains(index) && m_Attributes[index]->Name == _targetAttributeName)
        {
            m_Attributes.erase(index);
            return;
        }
    }
    LOG(logWarning, LogRHIVertexLayout, "Attribute name not used in this layout!");
}

void MirageRHI::OpenGLVertexLayout::ClearAttributes() { m_Attributes.clear(); }

void MirageRHI::OpenGLVertexLayout::SetLayout()
{
    glBindVertexArray(m_VAO);

    for (uint32_t index = 0; index < m_Attributes.size(); ++index)
    {
        if (!m_Attributes.contains(index))
        {
            LOG(logError, LogRHIVertexLayout, "Missing Attributes or Index Mismatch!");
            return;
        }

        GLboolean normalized = m_Attributes[index]->Normalized;
        GLenum type;

        switch (m_Attributes[index]->Type)
        {
            case AttributeType::FLOAT:
                type = GL_FLOAT;
                break;
            case AttributeType::INT:
                type = GL_INT;
                break;
            default:
                type = GL_FLOAT;
                break;
        }

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, m_Attributes[index]->Size, type, normalized, m_Attributes[index]->Stride, m_Attributes[index]->Pointer);
    }
}
