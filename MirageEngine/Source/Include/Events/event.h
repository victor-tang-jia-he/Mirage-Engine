#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>
#include <typeindex>

namespace MirageEngine::Core::EventSystem
{
    class IEvent
    {
        public:

            virtual ~IEvent()                       = default;
            virtual std::type_index GetType() const = 0;
    };

    template <typename... Args> class Event : public IEvent
    {
        public:

            using Callback = std::function<void(Args...)>;

            explicit Event(Callback cb)
                : m_Callback(std::move(cb))
            {
            }

            std::type_index GetType() const override { return typeid(Event<Args...>); }

            void Trigger(Args... args) { m_Callback(args...); }

        private:

            Callback m_Callback;
    };

    class EventDispatcher
    {
        public:

            template <typename... Args> void Add(std::shared_ptr<Event<Args...>> event) { m_Events[typeid(Event<Args...>)].push_back(event); }

            template <typename... Args> void Invoke(Args... args)
            {
                auto it = m_Events.find(typeid(Event<Args...>));
                if (it == m_Events.end()) return;

                for (auto& ie : it->second) { static_cast<Event<Args...>*>(ie.get())->Trigger(args...); }
            }

            template <typename... Args> void Remove() { m_Events.erase(typeid(Event<Args...>)); }

        private:

            std::unordered_map<std::type_index, std::vector<std::shared_ptr<IEvent>>> m_Events;
    };
} // namespace MirageEngine::Core::EventSystem
