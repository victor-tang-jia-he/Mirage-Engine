#pragma once

#include "i_uniform_buffer.h"
#include "matrix4.h"

namespace MirageRHI
{
    class OpenGLUniformBuffer : public IUniformBuffer
    {
    private:
        MirageEngine::Core::Math::Matrix4 m_Model;
        MirageEngine::Core::Math::Matrix4 m_View;
        MirageEngine::Core::Math::Matrix4 m_Proj;

    public:
        void Create() override;
        void Destroy() override {}

        void UpdateUniformBuffer(
            IPipeline* _pipeline,
            MirageEngine::Core::Math::Vector3 _position,
            MirageEngine::Core::Math::Vector3 _scale,
            MirageEngine::Core::Math::Vector3 _rotation) override;
    };
}