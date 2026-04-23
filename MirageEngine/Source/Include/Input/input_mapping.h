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

                entt::id_type GetType() const override { return entt::type_id<InputMapping>().hash(); }

                void RegisterInput(const std::string& name, int actionInput)
                {
                    // Register all input to the list Key To String
                    m_KeyToStrings[actionInput].push_back(name);

                    if (m_Map.find(name) == m_Map.end()) { m_Map[name] = std::make_shared<InputBind>(); }
                }

                void RegisterAxis1D(const std::string& name, int posX, int negX)
                {
                    // Register all input to the list Key To String
                    m_KeyToStrings[posX].push_back(name);
                    m_KeyToStrings[negX].push_back(name);
                    m_Axis1D[name] = { posX, negX, 0.f };
                    if (m_Map.find(name) == m_Map.end()) { m_Map[name] = std::make_shared<InputBind>(); }
                }

                void RegisterAxis2D(const std::string& name, int posX, int negX, int posY, int negY)
                {
                    // Register all input to the list Key To String
                    m_KeyToStrings[posX].push_back(name);
                    m_KeyToStrings[negX].push_back(name);
                    m_KeyToStrings[posY].push_back(name);
                    m_KeyToStrings[negY].push_back(name);
                    m_Axis2D[name] = { posX, negX, posY, negY };
                    if (m_Map.find(name) == m_Map.end()) { m_Map[name] = std::make_shared<InputBind>(); }
                }

                template <typename... Args> void RegisterAction(const std::string& name, std::function<void(Args...)> func);

                template <typename T, typename... Args> void RegisterAction(const std::string& name, T* instance, void (T::*method)(Args...))
                {
                    auto func = [instance, method](Args... args) { (instance->*method)(args...); };

                    RegisterAction<Args...>(name, func);
                }

                void Remove(const std::string& name);

                void Clear() { m_Map.clear(); }

                void BindMapping();

                void Init() override {}

                void Update() override {}
        };
    } // namespace Input
} // namespace MirageEngine::Core

#include <Input/input_mapping.inl>
