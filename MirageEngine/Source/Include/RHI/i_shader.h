#pragma once

#include <iostream>

DECLARE_LOG_CATEGORY_EXTERN(LogRHIShader)

namespace MirageRHI
{
    class OpenGLShader;

    class IShader
    {
        public:

            enum class ShaderType
            {
                None,
                Vertex,
                Fragment
            };

            virtual ~IShader() = default;

            virtual bool Create(const char* _shaderPath, ShaderType _type) = 0;
            virtual void Destroy() = 0;

            virtual OpenGLShader& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLShader"); }
    };
} // namespace MirageRHI
