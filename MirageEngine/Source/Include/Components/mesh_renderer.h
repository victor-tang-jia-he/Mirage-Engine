#pragma once

#include <Components/transform.h>
#include <Resources/material.h>
#include <Resources/mesh.h>

namespace MirageRHI
{
	class ICommandBuffer;
	class IUniformBuffer;
	class IPipeline;
}

namespace MirageEngine
{
	namespace Core
	{
		namespace Components
		{
			class MeshRenderer {
			public:
				MirageEngine::Core::Resources::IMesh* Mesh = nullptr;
				MirageEngine::Core::Resources::Material* Material = nullptr;
				MirageEngine::Core::Components::Transform Transform;


				MeshRenderer() = default;
				MeshRenderer(MirageEngine::Core::Resources::IMesh* _mesh, MirageEngine::Core::Resources::Material* _material)
					: Mesh(_mesh), Material(_material) {
				}

				void SetPos(MirageEngine::Core::Math::Vector3 _pos) { Transform.SetPosition(_pos); }
				void SetScale(MirageEngine::Core::Math::Vector3 _scale) { Transform.SetScale(_scale); }
				void SetRotation(MirageEngine::Core::Math::Quaternion _rotation) { Transform.SetRotation(_rotation); }

				void Render(MirageRHI::ICommandBuffer* _commandBuffer,MirageRHI::IPipeline* _pipeline, MirageRHI::IUniformBuffer* _uniformBuffer);
			};

		}
	}
}