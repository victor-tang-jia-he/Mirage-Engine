#pragma once

#include <vector>
#include <string>
#include <stdexcept>

namespace MirageEngine
{
	class Components
	{
	private:
		static std::vector<Components*> s_Components;
		std::string m_ComponentName;

	public:
		Components();
		virtual ~Components() = 0;

		static void AddComponent(Components* _component);
		static void RemoveComponent(Components* _component);

		// Getters
		std::string GetName() const { return m_ComponentName; }
	};

}
