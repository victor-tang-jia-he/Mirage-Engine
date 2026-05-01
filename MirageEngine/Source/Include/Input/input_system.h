#pragma once

#include <utility>
#include <unordered_set>
#include <Events/event.h>
#include <Input/input_system.h>
#include <Input/input_mapping.h>

struct GLFWwindow;

namespace MirageEngine::Core
{
    namespace Math
    {
        class Vector2;
    }

    namespace Input
    {
        struct InputBind;
        class InputMapping;

        class InputSystem
        {
            public:
            protected:
            private:

                static std::shared_ptr<InputMapping>   m_CurrentMap;
                static std::unordered_set<std::string> m_HeldButton;

                InputSystem& operator=(const InputSystem&) = delete;

                static Math::Vector2 m_LastMousePos;
                static Math::Vector2 m_MouseDelta;

            public:

                InputSystem()                   = delete;
                ~InputSystem()                  = delete;
                InputSystem(const InputSystem&) = delete;

                static void Initialize(std::shared_ptr<InputMapping> _map = nullptr);

                static void Shutdown();

                static void UpdateHeldInput();

                static Math::Vector2 GetMouseDelta();

                static void KeyCallback(GLFWwindow*, int _key, int, int _action, int);

                static void MouseButtonCallback(GLFWwindow*, int _button, int _action, int _mods);

                static void CursorPosCallback(GLFWwindow*, double _xpos, double _ypos);

                static std::shared_ptr<InputMapping> GetCurrentMapping() { return m_CurrentMap; }

                static void SetCurrentMapping(std::shared_ptr<InputMapping> _map);
            private:
                // Axis retrieval functions
                static void GetAxis1DFromKey(const std::string& _name, int _key, int _action, float& _currentValue);
                static void GetAxis2DFromKey(const std::string& _name, int _key, int _action, Math::Vector2& _currentValue);
        };
    } // namespace Input
} // namespace MirageEngine::Core
