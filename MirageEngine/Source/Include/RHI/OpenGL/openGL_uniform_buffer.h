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
		
	public:
		void Create() override;
		void Destroy() override {}

		void SetView(MirageEngine::Core::Math::Matrix4 _view) override { m_View = _view; }
		void SetProjection(MirageEngine::Core::Math::Matrix4 _proj) override { m_Proj = _proj; }
		void UpdateUniformBuffer(IPipeline* _pipeline, MirageEngine::Core::Math::Matrix4 _model) override;
	};
}