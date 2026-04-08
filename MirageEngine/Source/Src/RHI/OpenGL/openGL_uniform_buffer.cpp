#include "OpenGL/openGL_uniform_buffer.h"
#include "OpenGL/openGL_pipeline.h"

DEFINE_LOG_CATEGORY(LogRHIUniformBuffer)

using namespace MirageEngine::Core::Math;

void MirageRHI::OpenGLUniformBuffer::Create()
{
	m_View = Matrix4::Identity();
	m_Proj = Matrix4::Identity();

	LOG(logTemp, LogRHIUniformBuffer, "Uniform Buffer Created.");
}

void MirageRHI::OpenGLUniformBuffer::UpdateUniformBuffer(IPipeline* _pipeline, MirageEngine::Core::Math::Matrix4 _model)
{
	_pipeline->API_OpenGL().SetMat4("model", _model);
	_pipeline->API_OpenGL().SetMat4("view", m_View);
	_pipeline->API_OpenGL().SetMat4("proj", m_Proj);
}
