#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <Components/component.h>
#include <Components/transform.h>

namespace MirageEngine
{

	// Static : Transform matrix doesn't update every frame
	// Dynamic : Transform matrix updates every frame

	class GameObject
	{
	private:
		// Game Object variables
		std::string m_Name = "";
		std::vector<MirageEngine::Core::Components::Component*> m_Components;
		std::vector<GameObject*> m_Childrens;
		GameObject* m_Parent;

		// Editor parameters
		bool m_bIsActive = true;
		bool m_bIsStatic = false;

	public:
		MirageEngine::Core::Components::Transform* m_Transform = nullptr;

	public:
		// Game Object Constructor for create default Game Object or with name and other parameters.
		GameObject(GameObject* _parent, std::string _name);
		GameObject(GameObject* _parent, std::string _name, bool _isStatic);
		GameObject(GameObject* _parent, std::string _name, bool _isStatic, bool _isActive);

		~GameObject();

		void SetActive(bool _isActive);

		// Components Methods
		template <typename T>
		T* AddComponent()
		{
			T* component = new T();
			component->m_Parent = this;
			// Name of the type ID
			component->m_ComponentName = typeid(T).name();
			m_Components.push_back(component);
			return component;
		}

		void RemoveComponent(MirageEngine::Core::Components::Component* _component);
		MirageEngine::Core::Components::Component* GetComponent(std::string _name);

		// Getters
		std::string GetName() const { return m_Name; }
		bool GetIsActive() const { return m_bIsActive; }
		GameObject* GetParent() const { return m_Parent; }

	
	};
}
