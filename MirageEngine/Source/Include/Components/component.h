#pragma once

#include <stdexcept>

namespace entt
{
    using id_type = std::uint32_t;
    class meta_any;
}

namespace MirageEngine
{
    class GameObject;

	namespace Core::Components
    {
        class Component
        {
            friend class MirageEngine::GameObject;

        protected:
            std::string m_ComponentName;
            GameObject* m_Owner;

        public:
            Component() = default;
            Component(GameObject* _owner) : m_Owner(_owner) {}
            virtual ~Component() = default;

            virtual void OnStart() {};
            virtual void OnUpdate(float dt) {};
            virtual void OnLateUpdate(float dt) {};
            virtual void OnFixedUpdate(float dt) {};
            virtual void OnDrawLine() {};
            virtual void OnCreate() {}
            virtual void OnDestroy() {}
            virtual entt::id_type GetType() const = 0;

            // Getters
            std::string GetName() const { return m_ComponentName; }
            GameObject* GetOwner() const { return m_Owner; }
        };
    } // namespace Core::Components
} // namespace MirageEngine
