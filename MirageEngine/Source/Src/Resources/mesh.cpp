#include <Resources/mesh.h>
#include <RHI/OpenGL/openGL_buffer.h>
#include <RHI/i_buffer.h>

using namespace MirageEngine::Core::Resources;

void IMesh::Bind() const
{
	m_VertexBuffer->Bind();
	m_IndicesBuffer->Bind();
}