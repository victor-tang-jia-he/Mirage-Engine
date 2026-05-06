#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <Components/component.h>
#include <Components/transform.h>
#include <Components/mesh_renderer.h>
#include <Scripting/engine_script_api.h>

DECLARE_LOG_CATEGORY_EXTERN(LogGameObject)

namespace MirageEngine
{
	namespace Core::Math
	{
		class Vector3;
		class Quaternion;
	}

	// Static : Transform matrix doesn't update every frame
	// Dynamic : Transform matrix updates every frame

	class ENGINE_API GameObject  : public std::enable_shared_from_this<GameObject>
	{
	private:
		// Game Object variables
		std::string m_Name = "";
		std::vector<std::shared_ptr<MirageEngine::Core::Components::Component>> m_Components;
		std::vector<std::shared_ptr<GameObject>> m_Children;
		std::vector<std::string> m_ScriptNames;
		std::weak_ptr<GameObject> m_Parent;
		uint64_t m_ID;

		// Editor parameters
		bool m_bIsActive = true; // Do components update every frame
		bool m_bIsStatic = false; // Does transform update every frame
		bool m_bIsDirty = false;

		bool m_bIsRoot = false;

	public:

		MirageEngine::Core::Components::Transform Transform;

		template <typename T>
		std::shared_ptr<T> AddComponent();

		template <typename T>
		std::shared_ptr<T> GetComponent();

			// Game Object Constructor.
		GameObject(std::shared_ptr<GameObject> _parent, std::string _name, bool _isStatic = false, bool _isActive = true);
		~GameObject();

		// Game Object Custom Methods
		void AddChild(std::shared_ptr<GameObject> _child, bool _isStatic = false, bool _isActive = true);
		void SetActive(bool _isActive);

		void AddComponent(std::shared_ptr<MirageEngine::Core::Components::Component> _component);

		void RemoveComponent(std::shared_ptr<MirageEngine::Core::Components::Component> _component);

		// Game Object Public Methods
		bool IsRoot() const { return m_Parent.expired(); };
		void RemoveChild(std::shared_ptr<GameObject> _child);
		void SetDirty(bool _isDirty);

        // Hot reload
        void OnPreReload();
        void OnPostReload();

		// Getters
		std::string GetName() const { return m_Name; }
		std::vector<std::shared_ptr<GameObject>> GetChildren() const { return m_Children; }
		std::shared_ptr<GameObject> GetParent() const { return m_Parent.lock(); }
		uint64_t GetID() const { return m_ID; }
		bool GetIsStatic() const { return m_bIsStatic; }
		bool GetIsActive() const { return m_bIsActive; }
		bool GetIsDirty() const { return m_bIsDirty; }
		std::vector<std::shared_ptr<MirageEngine::Core::Components::Component>> GetComponents() const { return m_Components; }
		MirageEngine::Core::Math::Vector3 GetPosition() const { return Transform.GetPosition(); }
		MirageEngine::Core::Math::Quaternion GetRotation() const { return Transform.GetRotation(); }
		MirageEngine::Core::Math::Vector3 GetScale() const { return Transform.GetScale(); }

		// Setters
		void SetName(std::string _name) { m_Name = _name; }
		void SetChildren(std::vector<std::shared_ptr<GameObject>> _children) { m_Children = _children; }
		void SetParent(const std::shared_ptr<GameObject>& _parent) {  m_Parent = _parent; }
		void SetID(uint64_t _id) { m_ID = _id; }
		void SetIsStatic(bool _isStatic) { m_bIsStatic = _isStatic; }
		void SetIsActive(bool _isActive) { m_bIsActive = _isActive; }
		void SetRotation(MirageEngine::Core::Math::Quaternion _rotation);
		void SetPosition(MirageEngine::Core::Math::Vector3 _position);
		void SetScale(MirageEngine::Core::Math::Vector3 _scale);
		void SetPosition(float _x, float _y, float _z);
		void SetScale(float _x, float _y, float _z);
		
	}; // namespace MirageEngine
}

#include "game_object.inl"