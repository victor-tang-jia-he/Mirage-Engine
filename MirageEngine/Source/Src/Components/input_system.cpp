#include <Components/input_system.h>

namespace MirageEngine::Core::Input
{
    std::shared_ptr<InputMapping>       InputSystem::_currentMap = nullptr;
    std::unordered_map<int, std::weak_ptr<InputBind>> InputSystem::m_heldButton;

    void InputSystem::Initialize(std::shared_ptr<InputMapping>   map )
    {
        if (_currentMap != nullptr) { _currentMap.reset(); }
        _currentMap = map;
    }

    void InputSystem::Shutdown()
    {
        m_heldButton.clear();
        m_heldButton = {}; // reset bucket
        _currentMap.reset();
    }

    void InputSystem::UpdateHeldInput()
    {
        for (auto& kv : m_heldButton) { kv.second.lock()->Dispatcher.Invoke("InputSystem", kv.second.lock()->InputInfo); }
    }

    void InputSystem::KeyCallback(GLFWwindow*, int key, int, int action, int)
    {
        if (_currentMap == nullptr) return;

        auto input = _currentMap->Map.find(key);
        if (input != _currentMap->Map.end())
        {
            std::shared_ptr<InputBind> inputBind = input->second;
            CallbackInputInfo& inputInfo = inputBind->InputInfo;

            if (action == GLFW_PRESS)
            {
                inputInfo.Pressed = true;
                inputInfo.Held    = true;
                inputBind->Dispatcher.Invoke("InputSystem", inputInfo);
                inputInfo.Pressed = false;
                m_heldButton[key] = inputBind;
            }
            else if (action == GLFW_RELEASE)
            {
                inputInfo.Released = true;
                inputInfo.Held     = false;
                inputBind->Dispatcher.Invoke("InputSystem", inputInfo);
                inputInfo.Released = false;
                m_heldButton.erase(key);
            }
        }
    }

    CallbackInputInfo* InputSystem::GetKeyStats(int key)
    {
        if (_currentMap == nullptr) return nullptr;

        auto event = _currentMap->Map.find(key);
        if (event != _currentMap->Map.end()) { return &event->second->InputInfo; }
        return nullptr;
    }

    void InputSystem::SetCurrentMapping(std::shared_ptr<InputMapping> map)
    {
        // if (_currentMap != nullptr && _currentMap != map)
        //{
        //	delete _currentMap;
        // }
        _currentMap = map;
    }

    void InputSystem::Add(int _key, std::shared_ptr<EventSystem::IEvent> _event)
    {
        if (_currentMap == nullptr) return;
        if (_event)
        {
            if (_currentMap->Map.find(_key) != _currentMap->Map.end()) { _currentMap->Map[_key]->Dispatcher.Add(_event); }
            else
            {
                _currentMap->Map[_key] = std::make_shared<InputBind>();
                _currentMap->Map[_key]->Dispatcher.Add(_event);
            }
        }
    }
} // namespace MirageEngine::Core::Input
