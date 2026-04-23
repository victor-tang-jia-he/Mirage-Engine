#pragma once

#include <utility>
#include <unordered_map>

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

                static std::shared_ptr<InputMapping>   _currentMap;
                static std::unordered_set<std::string> m_heldButton;

                InputSystem& operator=(const InputSystem&) = delete;

                static Math::Vector2 m_LastMousePos;
                static Math::Vector2 m_MouseDelta;

            public:

                InputSystem()                   = delete;
                ~InputSystem()                  = delete;
                InputSystem(const InputSystem&) = delete;

                static void Initialize(std::shared_ptr<InputMapping> map = nullptr);

                static void Shutdown();

                static void UpdateHeldInput();

                static Math::Vector2 GetMouseDelta();

                static void KeyCallback(GLFWwindow*, int key, int, int action, int);

                static void MouseButtonCallback(GLFWwindow*, int button, int action, int mods);

                static void CursorPosCallback(GLFWwindow*, double xpos, double ypos);

                static std::shared_ptr<InputMapping> GetCurrentMapping() { return _currentMap; }

                static void SetCurrentMapping(std::shared_ptr<InputMapping> map);
            private:
                // Axis retrieval functions
                static void GetAxis1DFromKey(const std::string& name, int key, int action, float& _currentValue);
                static void GetAxis2DFromKey(const std::string& name, int key, int action, Math::Vector2& _currentValue);
        };
    } // namespace Input
} // namespace MirageEngine::Core
