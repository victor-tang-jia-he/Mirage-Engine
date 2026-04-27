#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <i_pipeline_parameters.h>

namespace MirageRHI
{
	struct OpenGLPipelineParameters : public IPipelineParameters
	{
	private:
		CullingType m_CullingType = CullingType::None;
		bool m_bDepthTest = false;

	public:
		void Create(CullingType _cullingType = CullingType::Front, bool _depthTest = false) override;
		void Destroy() override {}

		OpenGLPipelineParameters& API_OpenGL() override { return *this; }

		void ApplyParameters() const;
	};
}