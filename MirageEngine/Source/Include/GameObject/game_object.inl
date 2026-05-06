#pragma once

namespace MirageEngine
{
	template <typename T>
	std::shared_ptr<T> GameObject::AddComponent()
	{
		// std::unique_ptr<T> component = std::make_unique<T>();
		T rawComp(this);
		std::shared_ptr<T> component = std::make_shared<T>(rawComp);

		// Name of the type ID
		component->m_ComponentName = typeid(T).name();
		m_Components.push_back(component);
		return component;
	}

	template <typename T>
	void GameObject::AddComponent(std::shared_ptr<MirageEngine::Core::Components::Component> _component)
	{
		_component->m_Owner = this;

		// Name of the type ID
		_component->m_ComponentName = typeid(T).name();
		m_Components.push_back(_component);
	}

	template <typename T>
	std::shared_ptr<T> GameObject::GetComponent()
	{
		for (std::shared_ptr<MirageEngine::Core::Components::Component> component : m_Components)
		{
			if (auto casted = std::dynamic_pointer_cast<T>(component))
			{
				return casted;
			}
		}

		return nullptr;
	}
}