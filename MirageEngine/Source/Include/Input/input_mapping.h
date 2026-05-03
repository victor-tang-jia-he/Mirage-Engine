#pragma once

#include <unordered_map>

#include <Events/event.h>
#include <Components/component.h>
#include <Logs/log_macro.h>
#include <vector2.h>

DECLARE_LOG_CATEGORY_EXTERN(logInputMapping)

namespace MirageEngine::Core
{
    namespace Components
    {
        class Component; // forward declaration
    }

    namespace Math
    {
        class Vector2;
    }

    namespace Input
    {
        class InputSystem;

        struct AxisBind1D
        {
                int   KeyPositive;
                int   KeyNegative;
                float Value = 0.f;
        };

        struct AxisBind2D
        {
                int KeyPosX, KeyNegX;
                int KeyPosY, KeyNegY;
        };

        struct CallbackInputInfo
        {
                bool Pressed  = false;
                bool Held     = false;
                bool Released = false;

                float         Axis1D = 0.f;
                Math::Vector2 Axis2D = { 0, 0 };
        };

        struct InputBind
        {
                EventSystem::EventDispatcher Dispatcher;
                CallbackInputInfo            InputInfo;
                int                          HeldCount = 0;
        };

        struct InputMapping : public Components::Component, public std::enable_shared_from_this<InputMapping>
        {
                // Map name
                std::string m_MappingName = "default";

                // Map Key to Action name
                std::unordered_map<int, std::vector<std::string>> m_KeyToStrings;

                // Map Action
                std::unordered_map<std::string, std::shared_ptr<InputBind>> m_Map;

                std::unordered_map<std::string, AxisBind1D> m_Axis1D;
                std::unordered_map<std::string, AxisBind2D> m_Axis2D;

                InputMapping() = default;

                ~InputMapping() override;

                entt::id_type GetType() const override;

                void RegisterInput(const std::string& _name, int _actionInput);

                void RegisterAxis1D(const std::string& _name, int _posX, int _negX);

                void RegisterAxis2D(const std::string& _name, int _posX, int _negX, int _posY, int _negY);

                template <typename... Args> void RegisterAction(const std::string& _name, std::function<void(Args...)> _func);

                template <typename T, typename... Args> void RegisterAction(const std::string& _name, T* _instance, void (T::*_method)(Args...));

                template <typename T> void RegisterAction(const std::string& _name, T&& _func);

                void Remove(const std::string& _name);

                void Clear() { m_Map.clear(); }

                void BindMapping();
        };
    } // namespace Input
} // namespace MirageEngine::Core

#include <Input/input_mapping.inl>
