#pragma once

#include <iostream>

#include <i_pipeline.h>

#include <vector3.h>
#include <Components/camera.h>
#include <RHI/ubo_structs.h>

DECLARE_LOG_CATEGORY_EXTERN(LogRHIUniformBuffer)

namespace MirageRHI
{
    class OpenGLUniformBuffer;

    enum class UBOBinding : int
    {
        PerFrame = 0,
        PerObject = 1,
        Lights = 2,
        Material = 3
    };

    class IUniformBuffer
    {
        public:

            virtual ~IUniformBuffer() = default;
            virtual void Create(UBOBinding _binding, size_t _size) = 0;
            virtual void Upload(const void* _data, size_t _size) = 0;
            virtual void Destroy() = 0;

            virtual int GetID() = 0;

            virtual OpenGLUniformBuffer& API_OpenGL() { throw std::runtime_error("Bad API Call"); }
    };
} // namespace MirageRHI
