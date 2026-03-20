#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "i_pipeline_parameters.h"

namespace MirageRHI
{
	struct OpenGLPipelineParameters : public IPipelineParameters
	{
	private:
		GLuint m_VAO;
		VertexType m_VertexType;
		CullingType m_CullingType;
		bool m_bDepthTest;

	public:
		void Create(VertexType _vertexType = VertexType::Simple, CullingType _cullingType = CullingType::Front, bool _depthTest = true) override;
		void Destroy() override {}

		OpenGLPipelineParameters& API_OpenGL() override { return *this; }

		void ApplyParameters();

		void SetLayout() const;

	};
}