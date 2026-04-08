#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <Components/component.h>
#include <Components/transform.h>
#include <Components/mesh_renderer.h>

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
		std::vector<GameObject*> m_Children;
		GameObject* m_Parent = nullptr;
		uint64_t m_ID;

		// Editor parameters
		bool m_bIsActive = true; // Do components update every frame
		bool m_bIsStatic = false; // Does transform update every frame
		bool m_bIsDirty = false;

		bool m_bIsRoot = false;

	public:

		MirageEngine::Core::Components::Transform Transform;

		//~GameObject();

			// Game Object Constructor.
		GameObject(GameObject* _parent, std::string _name, bool _isStatic = false, bool _isActive = true);

		// Game Object Custom Methods
		void AddChild(GameObject* _child, bool _isStatic = false, bool _isActive = true);
		void SetActive(bool _isActive);


		// Components Methods
		template <typename T> T* AddComponent()
		{
			// std::unique_ptr<T> component = std::make_unique<T>();
			T* component = new T();
			component->m_Owner = this;
			// Name of the type ID
			component->m_ComponentName = typeid(T).name();
			m_Components.push_back(component);
			return component;
		}

		void AddComponent(MirageEngine::Core::Components::Component* _component)
		{
			_component->m_Owner = this;
			m_Components.push_back(_component);
		}

		template <typename T>
		T* GetComponent()
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

		void RemoveComponent(MirageEngine::Core::Components::Component* _component);

		// Game Object Public Methods
		bool IsRoot() const { return m_Parent == nullptr; };
		void SetParent(GameObject* _parent);
		void RemoveChild(GameObject* _child);
		void SetDirty(bool _isDirty) { m_bIsDirty = _isDirty; }

		// Getters
		std::string GetName() const { return m_Name; }
		bool GetIsStatic() const { return m_bIsStatic; }
		bool GetIsActive() const { return m_bIsActive; }
		GameObject* GetParent() const { return m_Parent; }
		std::vector<GameObject*> GetChildren() const { return m_Children; }
		std::vector<MirageEngine::Core::Components::Component*> GetComponents() const { return m_Components; }
		uint64_t GetID() const { return m_ID; }
		void SetPosition(float _x, float _y, float _z);
		void SetPosition(MirageEngine::Core::Math::Vector3 _position);
	}; // namespace MirageEngine
}
