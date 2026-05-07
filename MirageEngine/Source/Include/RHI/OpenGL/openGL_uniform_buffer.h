#pragma once

#include <i_uniform_buffer.h>

#include <matrix4.h>
#include <Components/camera.h>

namespace MirageRHI
{
    class OpenGLUniformBuffer : public IUniformBuffer
    {
        private:

            MirageEngine::Core::Math::Matrix4 m_View;
            MirageEngine::Core::Math::Matrix4 m_Proj;
            MirageEngine::Core::Math::Vector3 m_CamPos;

        public:

            void Create() override;

            void Destroy() override {}

            void SetView(MirageEngine::Core::Math::Matrix4 _view) override { m_View = _view; }

            void SetProjection(MirageEngine::Core::Math::Matrix4 _proj) override { m_Proj = _proj; }

            void SetCamPos(MirageEngine::Core::Math::Vector3 _camPos) override;

            void UpdateUniformBuffer(std::shared_ptr<IPipeline> _pipeline, MirageEngine::Core::Math::Matrix4 _model) override;
    };
} // namespace MirageRHI
