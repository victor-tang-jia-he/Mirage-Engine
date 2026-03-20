#include <debug.h>

#include "OpenGL/openGL_uniform_buffer.h"
#include "OpenGL/openGL_pipeline.h"

using namespace MirageEngine::Core::Math;

void MirageRHI::OpenGLUniformBuffer::Create()
{
	m_Model = Matrix4::Identity();
	m_View = Matrix4::LookAt(Vector3(0.f, 0.f, 3.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
	m_Proj = Matrix4::GetPerspectiveMatrix(1920, 1080, 0.1f, 100.f, 90.f);
}

void MirageRHI::OpenGLUniformBuffer::UpdateUniformBuffer(
	IPipeline* _pipeline,
	MirageEngine::Core::Math::Vector3 _position,
	MirageEngine::Core::Math::Vector3 _scale,
	MirageEngine::Core::Math::Vector3 _rotation)
{
	_rotation *= (PI / 180.f);
	m_Model = Matrix4::CreateTransformMatrix(_position, _rotation, _scale);
	_pipeline->API_OpenGL().SetMat4("model", m_Model);
	_pipeline->API_OpenGL().SetMat4("view", m_View);
	_pipeline->API_OpenGL().SetMat4("proj", m_Proj);
}
