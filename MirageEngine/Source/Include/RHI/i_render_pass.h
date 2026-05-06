#pragma once

#include <iostream>

#include <vector4.h>

DECLARE_LOG_CATEGORY_EXTERN(LogRHIRenderPass)

namespace MirageRHI
{
    class OpenGLRenderPass;

    class IRenderPass
    {
        public:

            virtual ~IRenderPass() = default;

            virtual void Create(MirageEngine::Core::Math::Vector4 _color, float _depth) = 0;
            virtual void Destroy() = 0;

            virtual OpenGLRenderPass& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLRenderPass"); }

            virtual void Begin() = 0;
            virtual void End() = 0;
    };
} // namespace MirageRHI
