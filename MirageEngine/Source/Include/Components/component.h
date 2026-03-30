#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include <GameObject/game_object.h>

namespace MirageEngine
{
	class Component
	{
		friend class GameObject;

	private:
		std::string m_ComponentName;
		GameObject* m_Parent;

	public:
		Component() = default;
		virtual ~Component() = default;

		virtual void Init();
		virtual void Update();

		// Getters
		std::string GetName() const { return m_ComponentName; }
		GameObject* GetParent() const { return m_Parent; }
	};

}
