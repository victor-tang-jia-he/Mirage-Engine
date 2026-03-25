#include <debug.h>

#include "OpenGL/openGL_uniform_buffer.h"
#include "OpenGL/openGL_pipeline.h"

using namespace MirageEngine::Core::Math;

void MirageRHI::OpenGLUniformBuffer::Create()
{
	//m_Proj = Matrix4::GetPerspectiveMatrix(1920, 1080, 0.1f, 100.f, 90.f);
}

void MirageRHI::OpenGLUniformBuffer::UpdateUniformBuffer(
	IPipeline* _pipeline,
	const MirageEngine::Core::Components::Camera* _cam,
	MirageEngine::Core::Components::Transform& _transform)
{
	Matrix4 model = _transform.GetMatrix();
	_pipeline->API_OpenGL().SetMat4("model", model);
	_pipeline->API_OpenGL().SetMat4("view", Matrix4::LookAt(_transform.GetPosition(), _transform.Forward(), _transform.Up()));
	_pipeline->API_OpenGL().SetMat4("proj", _cam->GetPerspective());
}
