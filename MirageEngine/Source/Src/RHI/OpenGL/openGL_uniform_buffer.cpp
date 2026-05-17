#include "OpenGL/openGL_uniform_buffer.h"
#include "OpenGL/openGL_pipeline.h"
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogRHIUniformBuffer)

using namespace MirageEngine::Core::Math;

namespace MirageRHI
{
    void MirageRHI::OpenGLUniformBuffer::Create(UBOBinding _binding, size_t _size)
    {
        m_Binding = _binding;

        glGenBuffers(1, &m_UniformBufferId);
        glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferId);
        glBufferData(GL_UNIFORM_BUFFER, _size, NULL, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, (int)_binding, m_UniformBufferId);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void OpenGLUniformBuffer::Upload(const void* data, size_t size)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferId);

        void* ptr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

        if (ptr)
            memcpy(ptr, data, size);

        glUnmapBuffer(GL_UNIFORM_BUFFER);
    }
} // namespace MirageRHI
