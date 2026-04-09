#pragma once

#include <Resources/material.h>
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
				MirageEngine::Core::Resources::Mesh* m_Mesh = nullptr;
				MirageEngine::Core::Resources::Material* m_Material = nullptr;

			public:
				MeshRenderer() = default;
				MeshRenderer(MirageEngine::Core::Resources::Mesh* _mesh, MirageEngine::Core::Resources::Material* _material)
					: m_Mesh(_mesh), m_Material(_material)
				{
				}

				void Init() override {};
				void Update() override {};

				entt::id_type GetType() const override { return entt::type_id<MeshRenderer>().hash(); }

				// Getters
				MirageEngine::Core::Resources::Mesh* GetMesh() const { return m_Mesh; }
				MirageEngine::Core::Resources::Material* GetMaterial() const { return m_Material; }

				// Setters
				void SetMesh(MirageEngine::Core::Resources::Mesh* _mesh) { m_Mesh = _mesh; }
				void SetMaterial(MirageEngine::Core::Resources::Material* _material) { m_Material = _material; }
			};
		}
	}
}