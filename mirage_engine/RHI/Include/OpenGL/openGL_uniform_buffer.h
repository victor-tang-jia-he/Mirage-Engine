#pragma once

#include "i_uniform_buffer.h"
#include "matrix4.h"
#include "../../Components/camera.h"

namespace MirageRHI
{
	class OpenGLUniformBuffer : public IUniformBuffer
	{
	public:
		void Create() override;
		void Destroy() override {}

		void UpdateUniformBuffer(
			IPipeline* _pipeline,
			const MirageEngine::Core::Components::Camera* cam,
			MirageEngine::Core::Components::Transform& transform) override;
	};
}