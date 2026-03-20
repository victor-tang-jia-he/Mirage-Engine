#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "i_pipeline_parameters.h"

namespace MirageRHI
{
	struct OpenGLPipelineParameters : public IPipelineParameters
	{
	private:
		GLuint m_VAO = 0;
		VertexType m_VertexType = VertexType::None;
		CullingType m_CullingType = CullingType::None;
		bool m_bDepthTest = false;

	public:
		void Create(VertexType _vertexType = VertexType::Simple, CullingType _cullingType = CullingType::Front, bool _depthTest = false) override;
		void Destroy() override {}

		OpenGLPipelineParameters& API_OpenGL() override { return *this; }

		void ApplyParameters();

		void SetLayout() const;

	};
}