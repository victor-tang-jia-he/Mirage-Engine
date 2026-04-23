#include <Input/input_mapping.h>
#include <Input/input_system.h>

DEFINE_LOG_CATEGORY(logInputMapping)

namespace MirageEngine::Core::Input
{
    InputMapping ::~InputMapping()
    {
        m_Map.clear();
        LOG(logTemp, logInputMapping, "Input Mapping destroyed : ", m_ComponentName);
    }

    void InputMapping::BindMapping() { InputSystem::SetCurrentMapping(shared_from_this()); }

    void InputMapping::Remove(const std::string& name)
    {
        // remove name from m_KeyToStrings
        for (auto it = m_KeyToStrings.begin(); it != m_KeyToStrings.end();)
        {
            auto& vec = it->second;
            vec.erase(std::remove(vec.begin(), vec.end(), name), vec.end());

            if (vec.empty()) it = m_KeyToStrings.erase(it);
            else ++it;
        }

        m_Map.erase(name);
        m_Axis1D.erase(name);
        m_Axis2D.erase(name);
    }
} // namespace MirageEngine::Core::Input
