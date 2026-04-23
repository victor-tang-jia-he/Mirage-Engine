#include <Input/input_system.h>
#include <GLFW/glfw3.h>
#include <vector2.h>

namespace MirageEngine::Core::Input
{
    std::shared_ptr<InputMapping>   InputSystem::_currentMap = nullptr;
    std::unordered_set<std::string> InputSystem::m_heldButton;

    Math::Vector2 InputSystem::m_LastMousePos;
    Math::Vector2 InputSystem::m_MouseDelta;

    void InputSystem::Initialize(std::shared_ptr<InputMapping> map)
    {
        if (_currentMap != nullptr) { _currentMap = nullptr; }
        _currentMap = map;
    }

    void InputSystem::Shutdown()
    {
        m_heldButton.clear();
        m_heldButton = {}; // reset bucket
        _currentMap  = nullptr;
    }

    void InputSystem::UpdateHeldInput()
    {
        if (!_currentMap) return;

        for (const auto& name : m_heldButton)
        {
            _currentMap->m_Map[name]->Dispatcher.Invoke(_currentMap->m_Map[name]->InputInfo);
        }
    }

    void InputSystem::KeyCallback(GLFWwindow*, int key, int, int action, int)
    {
        if (!_currentMap) return;
        if (action != GLFW_PRESS && action != GLFW_RELEASE) return;
        // Check if key string exist
        auto input = _currentMap->m_KeyToStrings.find(key);
        if (input == _currentMap->m_KeyToStrings.end()) return;

        // Get all string name related to the key
        std::vector<std::string> inputStringList = input->second;

        // Loop on all string name
        for (std::string name : inputStringList)
        {
            CallbackInputInfo& inputInfo = _currentMap->m_Map[name]->InputInfo;
            // Update axis values
            InputSystem::GetAxis1DFromKey(name, key, action, inputInfo.Axis1D);
            InputSystem::GetAxis2DFromKey(name, key, action, inputInfo.Axis2D);

            if (action == GLFW_PRESS)
            {
                _currentMap->m_Map[name]->HeldCount++; // Increment held count for the action
                inputInfo.Pressed = true;
                inputInfo.Held    = true;
                m_heldButton.insert(name);
                _currentMap->m_Map[name]->Dispatcher.Invoke(inputInfo);
                inputInfo.Pressed = false;
            }
            else
            {
                _currentMap->m_Map[name]->HeldCount--; // Decrement held count for the action
                if (_currentMap->m_Map[name]->HeldCount > 0) return; // If the action is still held by another key, do not trigger release

                inputInfo.Released = true;
                inputInfo.Held     = false;
                m_heldButton.erase(name);
                _currentMap->m_Map[name]->Dispatcher.Invoke(inputInfo);
                inputInfo.Released = false;
            }
        }
    }

    void InputSystem::MouseButtonCallback(GLFWwindow*, int button, int action, int mods) { KeyCallback(nullptr, button, 0, action, mods); }

    void InputSystem::GetAxis1DFromKey(const std::string& name, int key, int action, float& _currentValue)
    {
        if (!_currentMap) return;
        if (_currentMap->m_Axis1D.find(name) == _currentMap->m_Axis1D.end()) return;
        if (action == GLFW_PRESS)
        {
            if (key == _currentMap->m_Axis1D[name].KeyNegative) _currentValue += -1.f;
            if (key == _currentMap->m_Axis1D[name].KeyPositive) _currentValue += 1.f;
        }
        else
        {
            if (key == _currentMap->m_Axis1D[name].KeyNegative) _currentValue -= -1.f;
            if (key == _currentMap->m_Axis1D[name].KeyPositive) _currentValue -= 1.f;
        }
        _currentValue = std::clamp(_currentValue, -1.f, 1.f);
    }

    void InputSystem::GetAxis2DFromKey(const std::string& name, int key, int action, Math::Vector2& _currentValue)
    {
        if (!_currentMap) return;
        if (_currentMap->m_Axis2D.find(name) == _currentMap->m_Axis2D.end()) return;
        if (action == GLFW_PRESS)
        {
            if (key == _currentMap->m_Axis2D[name].KeyNegX) _currentValue += { -1.f, 0.f };
            if (key == _currentMap->m_Axis2D[name].KeyNegY) _currentValue += { 0.f, -1.f };
            if (key == _currentMap->m_Axis2D[name].KeyPosX) _currentValue += { 1.f, 0.f };
            if (key == _currentMap->m_Axis2D[name].KeyPosY) _currentValue += { 0.f, 1.f };
        }
        else
        {
            if (key == _currentMap->m_Axis2D[name].KeyNegX) _currentValue -= { -1.f, 0.f };
            if (key == _currentMap->m_Axis2D[name].KeyNegY) _currentValue -= { 0.f, -1.f };
            if (key == _currentMap->m_Axis2D[name].KeyPosX) _currentValue -= { 1.f, 0.f };
            if (key == _currentMap->m_Axis2D[name].KeyPosY) _currentValue -= { 0.f, 1.f };
        }
        _currentValue.X = std::clamp(_currentValue.X, -1.f, 1.f);
        _currentValue.Y = std::clamp(_currentValue.Y, -1.f, 1.f);
    }

    void InputSystem::CursorPosCallback(GLFWwindow*, double xpos, double ypos)
    {
        MirageEngine::Core::Math::Vector2 current = { (float)xpos, (float)ypos };
        m_MouseDelta                              = current - m_LastMousePos;
        m_LastMousePos                            = current;
    }

    MirageEngine::Core::Math::Vector2 InputSystem::GetMouseDelta()
    {
        MirageEngine::Core::Math::Vector2 delta = m_MouseDelta;
        m_MouseDelta                            = { 0, 0 };
        return delta;
    }

    void InputSystem::SetCurrentMapping(std::shared_ptr<InputMapping> map)
    {
        m_heldButton.clear();
        _currentMap = map;
    }
} // namespace MirageEngine::Core::Input
