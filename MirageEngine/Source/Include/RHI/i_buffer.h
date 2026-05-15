#pragma once

#include <iostream>
#include <filesystem>

DECLARE_LOG_CATEGORY_EXTERN(LogRHIBuffer)

namespace MirageRHI
{
    class OpenGLBuffer;

    class IBuffer
    {
    public:
        enum BufferType
        {
            NONE,
            VERTICES,
            VERTICES_DYNAMIC,
            INDICES,
        };

        virtual ~IBuffer() = default;

        virtual void Create(const void* _data, size_t _size, BufferType _type) = 0;
        virtual void Update(const void* _data, size_t _size) = 0;
        virtual void Destroy() = 0;

        virtual void Bind() const = 0;

        virtual OpenGLBuffer& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLBuffer"); }
    };
} // namespace MirageRHI
