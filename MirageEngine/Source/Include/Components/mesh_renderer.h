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
				MirageEngine::Core::Resources::Mesh* Mesh = nullptr;
				MirageEngine::Core::Resources::Material* Material = nullptr;

			public:
				MeshRenderer() = default;
				MeshRenderer(MirageEngine::Core::Resources::Mesh* _mesh, MirageEngine::Core::Resources::Material* _material)
					: Mesh(_mesh), Material(_material)
				{
				}

				void Init() override {};
				void Update() override {};

				// Getters
				MirageEngine::Core::Resources::Mesh* GetMesh() const { return Mesh; }
				MirageEngine::Core::Resources::Material* GetMaterial() const { return Material; }

				// Setters
				void SetMesh(MirageEngine::Core::Resources::Mesh* _mesh) { Mesh = _mesh; }
				void SetMaterial(MirageEngine::Core::Resources::Material* _material) { Material = _material; }
			};
		}
	}
}