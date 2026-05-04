#include "OpenGL/openGL_pipeline_parameters.h"
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogRHIPipelineParameters)

using namespace MirageEngine::Core::Math;

void MirageRHI::OpenGLPipelineParameters::Create(CullingType _cullingType, bool _depthTest)
{
	m_CullingType = _cullingType;
	m_bDepthTest = _depthTest;

	LOG(logTemp, LogRHIPipelineParameters, "Pipeline Parameters Created");
}

void MirageRHI::OpenGLPipelineParameters::ApplyParameters() const
{
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
