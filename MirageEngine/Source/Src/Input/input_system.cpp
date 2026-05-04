#include <Input/input_system.h>
#include <GLFW/glfw3.h>
#include <vector2.h>
#include <algorithm>

namespace MirageEngine::Core::Input
{
    std::shared_ptr<InputMapping>   InputSystem::m_CurrentMap = nullptr;
    std::unordered_set<std::string> InputSystem::m_HeldButton;

    Math::Vector2 InputSystem::m_LastMousePos;
    Math::Vector2 InputSystem::m_MouseDelta;

    void InputSystem::Initialize(std::shared_ptr<InputMapping> _map)
    {
        if (m_CurrentMap != nullptr) { m_CurrentMap = nullptr; }
        m_CurrentMap = _map;
    }

    void InputSystem::Shutdown()
    {
        m_HeldButton.clear();
        m_HeldButton = {}; // reset bucket
        m_CurrentMap = nullptr;
    }

    void InputSystem::UpdateHeldInput()
    {
        if (!m_CurrentMap) return;

        for (const auto& name : m_HeldButton) { m_CurrentMap->m_Map[name]->Dispatcher.Invoke(m_CurrentMap->m_Map[name]->InputInfo); }
    }

    void InputSystem::KeyCallback(GLFWwindow*, int _key, int, int _action, int)
    {
        if (!m_CurrentMap) return;
        if (_action != GLFW_PRESS && _action != GLFW_RELEASE) return;
        // Check if key string exist
        auto input = m_CurrentMap->m_KeyToStrings.find(_key);
        if (input == m_CurrentMap->m_KeyToStrings.end()) return;

        // Get all string name related to the key
        std::vector<std::string> inputStringList = input->second;

        // Loop on all string name
        for (std::string name : inputStringList)
        {
            CallbackInputInfo& inputInfo = m_CurrentMap->m_Map[name]->InputInfo;
            // Update axis values
            InputSystem::GetAxis1DFromKey(name, _key, _action, inputInfo.Axis1D);
            InputSystem::GetAxis2DFromKey(name, _key, _action, inputInfo.Axis2D);

            if (_action == GLFW_PRESS)
            {
                m_CurrentMap->m_Map[name]->HeldCount++; // Increment held count for the action
                inputInfo.Pressed = true;
                inputInfo.Held    = true;
                m_HeldButton.insert(name);
                m_CurrentMap->m_Map[name]->Dispatcher.Invoke(inputInfo);
                inputInfo.Pressed = false;
            }
            else
            {
                m_CurrentMap->m_Map[name]->HeldCount--;               // Decrement held count for the action
                if (m_CurrentMap->m_Map[name]->HeldCount > 0) return; // If the action is still held by another key, do not trigger release

                inputInfo.Released = true;
                inputInfo.Held     = false;
                m_HeldButton.erase(name);
                m_CurrentMap->m_Map[name]->Dispatcher.Invoke(inputInfo);
                inputInfo.Released = false;
            }
        }
    }

    void InputSystem::MouseButtonCallback(GLFWwindow*, int _button, int _action, int _mods) { KeyCallback(nullptr, _button, 0, _action, _mods); }

    void InputSystem::GetAxis1DFromKey(const std::string& _name, int _key, int _action, float& _currentValue)
    {
        if (!m_CurrentMap) return;
        if (m_CurrentMap->m_Axis1D.find(_name) == m_CurrentMap->m_Axis1D.end()) return;
        float value = 1.f;
        if (_action == GLFW_RELEASE) value = -1.f;

        if (_key == m_CurrentMap->m_Axis1D[_name].KeyNegative) _currentValue += -value;
        if (_key == m_CurrentMap->m_Axis1D[_name].KeyPositive) _currentValue += value;
        _currentValue = std::clamp(_currentValue, -1.f, 1.f);
    }

    void InputSystem::GetAxis2DFromKey(const std::string& _name, int _key, int _action, Math::Vector2& _currentValue)
    {
        if (!m_CurrentMap) return;
        if (m_CurrentMap->m_Axis2D.find(_name) == m_CurrentMap->m_Axis2D.end()) return;
        float value = 1.f;
        if (_action == GLFW_RELEASE) value = -1.f;

        if (_key == m_CurrentMap->m_Axis2D[_name].KeyNegX) _currentValue += { -value, 0.f };
        if (_key == m_CurrentMap->m_Axis2D[_name].KeyNegY) _currentValue += { 0.f, -value };
        if (_key == m_CurrentMap->m_Axis2D[_name].KeyPosX) _currentValue += { value, 0.f };
        if (_key == m_CurrentMap->m_Axis2D[_name].KeyPosY) _currentValue += { 0.f, value };

        _currentValue.X = std::clamp(_currentValue.X, -1.f, 1.f);
        _currentValue.Y = std::clamp(_currentValue.Y, -1.f, 1.f);
    }

    void InputSystem::CursorPosCallback(GLFWwindow*, double _xpos, double _ypos)
    {
        MirageEngine::Core::Math::Vector2 current = { (float)_xpos, (float)_ypos };
        m_MouseDelta                              = current - m_LastMousePos;
        m_LastMousePos                            = current;
    }

    MirageEngine::Core::Math::Vector2 InputSystem::GetMouseDelta()
    {
        MirageEngine::Core::Math::Vector2 delta = m_MouseDelta;
        m_MouseDelta                            = { 0, 0 };
        return delta;
    }

    void InputSystem::SetCurrentMapping(std::shared_ptr<InputMapping> _map)
    {
        m_HeldButton.clear();
        m_CurrentMap = _map;
    }
} // namespace MirageEngine::Core::Input
