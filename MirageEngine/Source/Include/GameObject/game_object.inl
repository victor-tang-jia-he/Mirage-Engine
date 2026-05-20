#pragma once

namespace MirageEngine
{
	template <typename T>
	std::shared_ptr<T> GameObject::AddComponent()
	{
		std::shared_ptr<T> component = std::make_shared<T>();

		component->m_Owner = this;

		// Name of the type ID
		component->m_ComponentName = typeid(T).name();
		m_Components.push_back(component);
		m_LastAddedComponent = component;
		return component;
	}

	template <typename T>
	void GameObject::AddComponent(std::shared_ptr<MirageEngine::Core::Components::Component> _component)
	{
		_component->m_Owner = this;

		// Name of the type ID
		_component->m_ComponentName = typeid(T).name();
		m_Components.push_back(_component);
		m_LastAddedComponent = _component;
	}

	template <typename T>
	bool GameObject::RemoveComponent()
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if (auto casted = std::dynamic_pointer_cast<T>(*it))
			{
				m_Components.erase(it);
				return true;
			}
		}

		return false;
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
} // namespace MirageEngine
