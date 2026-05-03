#pragma once

#include <Resources/material_instance.h>
#include <Resources/mesh.h>
#include <Components/component.h>

namespace MirageEngine
{
	namespace Core
	{
		namespace Components
		{
			class MeshRenderer : public Component
			{
			private:
				std::shared_ptr<MirageEngine::Core::Resources::Mesh> m_Mesh = nullptr;
				std::shared_ptr<MirageEngine::Core::Resources::MaterialInstance> m_Material = nullptr;

			public:
				MeshRenderer() = default;
				MeshRenderer(std::shared_ptr<MirageEngine::Core::Resources::Mesh> _mesh, std::shared_ptr<MirageEngine::Core::Resources::MaterialInstance> _material)
					: m_Mesh(_mesh), m_Material(_material)
				{
				}

				entt::id_type GetType() const override;

				// Getters
				std::shared_ptr<MirageEngine::Core::Resources::Mesh> GetMesh() const { return m_Mesh; }
				std::shared_ptr<MirageEngine::Core::Resources::MaterialInstance> GetMaterial() const { return m_Material; }

				// Setters
				void SetMesh(std::shared_ptr<MirageEngine::Core::Resources::Mesh> _mesh) { m_Mesh = _mesh; }
				void SetMaterial(std::shared_ptr<MirageEngine::Core::Resources::MaterialInstance> _material) { m_Material = _material; }
			};
		}
	}
}