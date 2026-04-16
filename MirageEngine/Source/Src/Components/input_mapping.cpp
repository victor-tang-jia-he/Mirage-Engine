#include <Components/input_mapping.h>
#include <Components/input_system.h>

DEFINE_LOG_CATEGORY(logInputMapping)

namespace MirageEngine::Core::Input
{
    InputMapping ::~InputMapping()
    {
        Map.clear();
        LOG(logTemp, logInputMapping, "Input Mapping destroyed : ", m_ComponentName);
    }

    void InputMapping::BindMapping() { InputSystem::SetCurrentMapping(shared_from_this()); }

    void InputMapping::Add(int key, std::shared_ptr<EventSystem::IEvent> event)
    {
        if (event)
        {
            if (Map.find(key) != Map.end()) { Map[key]->Dispatcher.Add(event); }
            else
            {
                Map[key] = std::make_shared<InputBind>();
                Map[key]->Dispatcher.Add(event);
            }
        }
    }
} // namespace MirageEngine::Core::Input
