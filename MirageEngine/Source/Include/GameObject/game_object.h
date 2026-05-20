#pragma once

#include <Components/transform.h>
#include <Components/mesh_renderer.h>
#include <Scripting/engine_script_api.h>

DECLARE_LOG_CATEGORY_EXTERN(LogGameObject)

namespace entt
{
    class meta_type;
}

namespace MirageEngine
{
    namespace Core::Math
    {
        class Vector3;
        class Quaternion;
    } // namespace Core::Math

    class ENGINE_API GameObject : public std::enable_shared_from_this<GameObject>
    {
    public:
        MirageEngine::Core::Components::Transform Transform;

    private:
	    std::shared_ptr<Core::Components::Component> m_LastAddedComponent;

        // Game Object variables
        std::string m_Name = "";
        std::vector<std::shared_ptr<Core::Components::Component>> m_Components;
        std::vector<std::shared_ptr<GameObject>> m_Children;
        std::vector<std::string> m_ScriptNames;
        std::weak_ptr<GameObject> m_Parent;
        uint64_t m_ID;

        // Editor parameters
        bool m_bIsActive = true;  // Do components update every frame
        bool m_bIsStatic = false; // Does transform update every frame
        bool m_bIsDirty = false;
        bool m_bIsPendingDestroy = false;

        bool m_bIsRoot = false;

        public:
            // Game Object Constructor.
            GameObject(std::shared_ptr<GameObject> _parent, std::string _name, bool _isStatic = false, bool _isActive = true);
            ~GameObject();

            void AddChild(std::shared_ptr<GameObject> _child, bool _isStatic = false, bool _isActive = true);

			void AddComponentFromMeta(entt::meta_any& _compInstance);
            void AddComponent(std::shared_ptr<Core::Components::Component> _component);
            void RemoveComponent(std::shared_ptr<Core::Components::Component> _component);

            std::shared_ptr<Core::Components::Component> AddComponentByMeta(entt::meta_type _type);

            // Game Object Public Methods
            bool IsRoot() const { return m_Parent.expired(); };

            void RemoveChild(std::shared_ptr<GameObject> _child);
            void SetDirty(bool _isDirty);

            // Hot reload
            void OnPreReload();
            void OnPostReload();

            void Destroy() { m_bIsPendingDestroy = true; }
            bool IsPendingDestroy() const { return m_bIsPendingDestroy; }

            // Getters
            std::string GetName() const { return m_Name; }
            std::vector<std::shared_ptr<GameObject>> GetChildren() const { return m_Children; }
            std::shared_ptr<GameObject> GetParent() const { return m_Parent.lock(); }
            uint64_t GetID() const { return m_ID; }
            bool GetIsStatic() const { return m_bIsStatic; }
            bool GetIsActive() const { return m_bIsActive; }
            bool GetIsDirty() const { return m_bIsDirty; }
            std::vector<std::shared_ptr<Core::Components::Component>> GetComponents() const { return m_Components; }
            std::shared_ptr<Core::Components::Component> GetLastAddedComponent() const { return m_LastAddedComponent; }

            Core::Math::Vector3 GetPosition() const { return Transform.GetPosition(); }
            Core::Math::Quaternion GetRotation() const { return Transform.GetRotation(); }
            Core::Math::Vector3 GetScale() const { return Transform.GetScale(); }

            // Setters
            void SetName(std::string _name) { m_Name = _name; }
            void SetChildren(std::vector<std::shared_ptr<GameObject>> _children) { m_Children = _children; }
            void SetParent(const std::shared_ptr<GameObject>& _parent) { m_Parent = _parent; }
            void SetID(uint64_t _id) { m_ID = _id; }
            void SetIsStatic(bool _isStatic) { m_bIsStatic = _isStatic; }
            void SetIsActive(bool _isActive) { m_bIsActive = _isActive; }
            void SetRotation(Core::Math::Quaternion _rotation);
            void SetPosition(Core::Math::Vector3 _position);
            void SetScale(Core::Math::Vector3 _scale);
            void SetPosition(float _x, float _y, float _z);
            void SetScale(float _x, float _y, float _z);

            template <typename T> std::shared_ptr<T> AddComponent();
            template <typename T> void AddComponent(std::shared_ptr<Core::Components::Component> _component);
            template <typename T> bool RemoveComponent();
            template <typename T> std::shared_ptr<T> GetComponent();

    }; // namespace MirageEngine
} // namespace MirageEngine

#include "game_object.inl"