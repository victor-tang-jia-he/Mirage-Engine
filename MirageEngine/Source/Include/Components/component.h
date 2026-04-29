#pragma once

#include <vector>
#include <string>
#include <stdexcept>

namespace entt {
	using id_type = std::uint32_t;
}
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

				virtual void OnCreate() {}
				virtual void OnDestroy() {}

				virtual entt::id_type GetType() const = 0;

				// Getters
				std::string GetName() const { return m_ComponentName; }
				GameObject* GetOwner() const { return m_Owner; }
			};
		}
	}
}
