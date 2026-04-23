#pragma once
#include <Input/input_mapping.h>

namespace MirageEngine::Core::Input
{
    template <typename... Args> void InputMapping::RegisterAction(const std::string& name, std::function<void(Args...)> func)
    {
        std::shared_ptr<EventSystem::Event<Args...>> event = std::make_shared<EventSystem::Event<Args...>>(func);
        if (m_Map.find(name) != m_Map.end()) { m_Map[name]->Dispatcher.Add(event); }
        else
        {
            m_Map[name] = std::make_shared<InputBind>();
            m_Map[name]->Dispatcher.Add(event);
        }
    }
}
