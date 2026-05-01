#include <Components/mesh_renderer.h>

#include <i_pipeline_parameters.h>

#include <entt/entt.hpp>

entt::id_type MirageEngine::Core::Components::MeshRenderer::GetType() const { return entt::type_id<MirageEngine::Core::Components::MeshRenderer>().hash(); }