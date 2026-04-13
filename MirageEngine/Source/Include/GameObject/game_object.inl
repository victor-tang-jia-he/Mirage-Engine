#pragma once

namespace MirageEngine
{
	template <typename T>
	T* GameObject::AddComponent()
	{
		// std::unique_ptr<T> component = std::make_unique<T>();
		T* component = new T();
		component->m_Owner = this;

		// Name of the type ID
		component->m_ComponentName = typeid(T).name();
		m_Components.push_back(component);
		return component;
	}

	template <typename T>
	T* GameObject::GetComponent()
	{
		for (auto& component : m_Components)
		{
			if (auto casted = dynamic_cast<T*>(component))
			{
				return casted;
			}
		}

		return nullptr;
	}
}