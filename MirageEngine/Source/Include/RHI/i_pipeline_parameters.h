#pragma once

#include <iostream>

#include <vector3.h>
#include <vector2.h>

DECLARE_LOG_CATEGORY_EXTERN(LogRHIPipelineParameters)

namespace MirageRHI
{
    struct OpenGLPipelineParameters;

    enum class CullingType
    {
        NONE,
        FRONT,
        BLACK
    };

    struct IPipelineParameters
    {
    public:
        virtual void Create(CullingType _cullingType, bool _depthTest) = 0;
        virtual void Destroy() = 0;

        virtual OpenGLPipelineParameters& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLPipelineParameters"); }
    };
} // namespace MirageRHI
