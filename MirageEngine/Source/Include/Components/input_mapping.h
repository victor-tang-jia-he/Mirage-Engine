#pragma once

#include <unordered_map>

#include <Events/event.h>
#include "component.h"

namespace MirageEngine
{
    namespace Core
    {
        namespace Components
        {
            class Component; // forward declaration
        }

        namespace Input
        {
            class InputSystem;
            struct CallbackInputInfo
            {
                    bool Pressed  = false;
                    bool Held     = false;
                    bool Released = false;
            };

            struct InputBind
            {
                    EventSystem::EventDispatcher Dispatcher;
                    CallbackInputInfo            InputInfo;
            };

            struct InputMapping : public Components::Component
            {
                    std::unordered_map<int, InputBind> Map;

                    InputMapping() = default;

                    ~InputMapping() override { Map.clear(); }

                    template <typename... Args> void Add(int key, std::function<void(Args...)> func)
                    {
                        EventSystem::Event<Args...>* event = new EventSystem::Event<Args...>("InputSystem", func);

                        if (Map.find(key) != Map.end()) { Map[key].Dispatcher.Add(event); }
                        else
                        {
                            Map[key] = InputBind{};
                            Map[key].Dispatcher.Add(event);
                        }
                    }

                    template <typename T, typename... Args> void Add(int key, T* instance, void (T::*method)(Args...))
                    {
                        auto func = [instance, method](Args... args) { (instance->*method)(args...); };

                        Add<Args...>(key, func);
                    }

                    void Add(int key, EventSystem::IEvent* event)
                    {
                        if (event)
                        {
                            if (Map.find(key) != Map.end()) { Map[key].Dispatcher.Add(event); }
                            else
                            {
                                Map[key] = InputBind{};
                                Map[key].Dispatcher.Add(event);
                            }
                        }
                    }

                    void Remove(int key) { Map.erase(key); }

                    void Clear() { Map.clear(); }

                    void BindMapping();

                    void Init() override {}

                    void Update() override {}
            };
        } // namespace Input
    } // namespace Core
} // namespace MirageEngine
