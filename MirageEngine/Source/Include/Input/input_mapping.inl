#pragma once
#include <Input/input_mapping.h>

namespace MirageEngine::Core::Input
{
    template <typename... Args> void InputMapping::RegisterAction(const std::string& _name, std::function<void(Args...)> _func)
    {
        std::shared_ptr<EventSystem::Event<Args...>> event = std::make_shared<EventSystem::Event<Args...>>(_func);
        if (m_Map.find(_name) != m_Map.end())
        {
            m_Map[_name]->Dispatcher.Add(event);
        }
        else
        {
            m_Map[_name] = std::make_shared<InputBind>();
            m_Map[_name]->Dispatcher.Add(event);
        }
    }

    template <typename T, typename... Args> void InputMapping::RegisterAction(const std::string& _name, T* _instance, void (T::*_method)(Args...))
    {
        auto func = [_instance, _method](Args... _args)
        {
            (_instance->*_method)(_args...);
        };

        RegisterAction<Args...>(_name, func);
    }

    template <typename T> void InputMapping::RegisterAction(const std::string& _name, T&& _func) { RegisterAction(_name, std::function(std::forward<T>(_func))); }
} // namespace MirageEngine::Core::Input
