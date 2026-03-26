#include <debug.h>

#include "OpenGL/openGL_uniform_buffer.h"
#include "OpenGL/openGL_pipeline.h"

using namespace MirageEngine::Core::Math;

void MirageRHI::OpenGLUniformBuffer::Create()
{
}

void MirageRHI::OpenGLUniformBuffer::UpdateUniformBuffer(
	IPipeline* _pipeline,
	const MirageEngine::Core::Components::Camera* _cam,
	MirageEngine::Core::Components::Transform& _transform)
{
	Matrix4 m_Model = Matrix4::CreateTransformMatrix(Vector3(2, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));

	//Matrix4 model = _transform.GetMatrix();
	_pipeline->API_OpenGL().SetMat4("model", m_Model);
	_pipeline->API_OpenGL().SetMat4("view", Matrix4::LookAt(_transform.GetPosition(), _transform.GetPosition() + _transform.Forward(), _transform.Up()));
	_pipeline->API_OpenGL().SetMat4("proj", _cam->GetPerspective());
}
