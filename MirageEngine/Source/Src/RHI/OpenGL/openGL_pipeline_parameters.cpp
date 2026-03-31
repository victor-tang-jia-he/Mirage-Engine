#include "OpenGL/openGL_pipeline_parameters.h"
#include "debug.h"

using namespace MirageEngine::Core::Math;

void MirageRHI::OpenGLPipelineParameters::Create(VertexType _vertexType, CullingType _cullingType, bool _depthTest)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	m_VertexType = _vertexType;
	m_CullingType = _cullingType;
	m_bDepthTest = _depthTest;
}

void MirageRHI::OpenGLPipelineParameters::ApplyParameters()
{
	SetLayout();

	if (m_CullingType == CullingType::None)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		switch (m_CullingType)
		{
		case CullingType::Front:
			glCullFace(GL_FRONT);
			break;
		case CullingType::Back:
			glCullFace(GL_BACK);
			break;
		}
	}

	if (m_bDepthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void MirageRHI::OpenGLPipelineParameters::SetLayout() const
{
	glBindVertexArray(m_VAO);

	switch (m_VertexType)
	{
	case MirageRHI::VertexType::None:
		break;
	case MirageRHI::VertexType::Simple:
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(UnlitVertex), (void*)offsetof(UnlitVertex, Position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void*)offsetof(UnlitVertex, Color));
		break;
	case MirageRHI::VertexType::PBR:
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LitVertex), (void*)offsetof(LitVertex, Position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LitVertex), (void*)offsetof(LitVertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(LitVertex), (void*)offsetof(LitVertex, UV));
		break;
	default:
		break;
	}
}
