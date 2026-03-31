#include <Components/mesh_renderer.h>
#include <RHI/i_command_buffer.h>
#include <RHI/i_uniform_buffer.h>

void MirageEngine::Core::Components::MeshRenderer::Render(MirageRHI::ICommandBuffer* _commandBuffer, MirageRHI::IPipeline* _pipeline, MirageRHI::IUniformBuffer* _uniformBuffer)
{
    if (!Mesh || !Material) return;
    Material->Bind();
    _uniformBuffer->UpdateUniformBuffer(_pipeline,Transform.GetMatrix());
    _commandBuffer->BindBuffer(Mesh->GetVertexBuffer());
    _commandBuffer->BindBuffer(Mesh->GetIndexBuffer());
    _commandBuffer->Draw((uint32_t)Mesh->GetIndicesCount());
}