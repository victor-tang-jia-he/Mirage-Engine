#include <Components/components.h>

using namespace MirageEngine;

Components::Components()
{
}

void Components::AddComponent(Components* _component)
{
	for (Components* composents : s_Components)
	{
		if (composents->GetName() == _component->GetName())
		{
			throw(std::runtime_error("Component with the same name already exists."));
		}
	}
	
	s_Components.push_back(_component);
}

void Components::RemoveComponent(Components* _component)
{
	s_Components.erase(std::remove(s_Components.begin(), s_Components.end(), _component), s_Components.end());
}