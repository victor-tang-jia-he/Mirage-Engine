#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include <entt/meta/factory.hpp>
#include <entt/meta/container.hpp>
#include <entt/meta/meta.hpp>

namespace MirageEngine
{
	class GameObject;
	namespace Core
	{
		namespace Components
		{
			class Component
			{
				friend class MirageEngine::GameObject;

			protected:
				std::string m_ComponentName;
				GameObject* m_Owner;

			public:
				Component() = default;
				virtual ~Component() = default;

				virtual void Init() = 0;
				virtual void Update() = 0;

				virtual entt::id_type GetType() const = 0;

				// Getters
				std::string GetName() const { return m_ComponentName; }
				GameObject* GetOwner() const { return m_Owner; }
			};
		}
	}
}
