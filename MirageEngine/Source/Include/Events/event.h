#pragma once

#include <typeindex>

namespace MirageEngine::Core::EventSystem
{
    // Base interface for all events stored in the dispatcher.
    // Provides runtime type information through GetType().
    class IEvent
    {
    public:

        virtual ~IEvent() = default;

        // Returns the unique runtime type of the event.
        virtual std::type_index GetType() const = 0;
    };

    // Generic event class storing a callback with arbitrary arguments.
    // Args... defines the parameter list passed during invocation.
    template <typename... Args> class Event : public IEvent
    {
    public:

        using Callback = std::function<void(Args...)>;

        // Construct an event from a callback function/lambda.
        explicit Event(Callback cb)
            : m_Callback(std::move(cb))
        {
        }

        // Returns the runtime type associated with this event signature.
        std::type_index GetType() const override
        {
            return typeid(Event<Args...>);
        }

        // Executes the stored callback with the provided arguments.
        void Trigger(Args... args)
        {
            m_Callback(args...);
        }

    private:

        Callback m_Callback;
    };

    // Central dispatcher responsible for storing and invoking events.
    class EventDispatcher
    {
    public:

        // Registers an event instance in the dispatcher.
        template <typename... Args>
        void Add(std::shared_ptr<Event<Args...>> event)
        {
            m_Events[typeid(Event<Args...>)].push_back(event);
        }

        // Invokes all registered events matching the given signature.
        template <typename... Args>
        void Invoke(Args... args)
        {
            auto it = m_Events.find(typeid(Event<Args...>));

            if (it == m_Events.end())
                return;

            for (auto& ie : it->second)
            {
                static_cast<Event<Args...>*>(ie.get())->Trigger(args...);
            }
        }

        // Removes all events matching the specified signature.
        template <typename... Args>
        void Remove()
        {
            m_Events.erase(typeid(Event<Args...>));
        }

    private:

        // Stores events grouped by their runtime type.
        std::unordered_map<std::type_index, std::vector<std::shared_ptr<IEvent>>> m_Events;
    };
} // namespace MirageEngine::Core::EventSystem