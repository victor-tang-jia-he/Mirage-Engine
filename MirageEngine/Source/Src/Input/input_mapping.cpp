#include <Input/input_mapping.h>
#include <Input/input_system.h>
#include <GameObject/game_object.h>

DEFINE_LOG_CATEGORY(logInputMapping)

namespace MirageEngine::Core::Input
{
    InputMapping ::~InputMapping()
    {
        m_Map.clear();
        LOG(logTemp, logInputMapping, "Input Mapping destroyed : ", m_ComponentName);
    }

    void InputMapping::BindMapping() { InputSystem::SetCurrentMapping(shared_from_this()); }

    void InputMapping::AttachMeta(std::shared_ptr<GameObject> _Owner)
    {
        std::shared_ptr<InputMapping> compPtr = std::make_shared<InputMapping>(*this);
        compPtr->m_ComponentName = typeid(InputMapping).name();
        _Owner->AddComponent(compPtr);
    }

    void InputMapping::RegisterInput(const std::string& _name, int _actionInput)
    {
        // Register all input to the list Key To String
        m_KeyToStrings[_actionInput].push_back(_name);

        if (m_Map.find(_name) == m_Map.end()) { m_Map[_name] = std::make_shared<InputBind>(); }
    }

    void InputMapping::RegisterAxis1D(const std::string& _name, int _posX, int _negX)
    {
        // Register all input to the list Key To String
        m_KeyToStrings[_posX].push_back(_name);
        m_KeyToStrings[_negX].push_back(_name);
        m_Axis1D[_name] = { _posX, _negX, 0.f };
        if (m_Map.find(_name) == m_Map.end()) { m_Map[_name] = std::make_shared<InputBind>(); }
    }

    void InputMapping::RegisterAxis2D(const std::string& _name, int _posX, int _negX, int _posY, int _negY)
    {
        // Register all input to the list Key To String
        m_KeyToStrings[_posX].push_back(_name);
        m_KeyToStrings[_negX].push_back(_name);
        m_KeyToStrings[_posY].push_back(_name);
        m_KeyToStrings[_negY].push_back(_name);
        m_Axis2D[_name] = { _posX, _negX, _posY, _negY };
        if (m_Map.find(_name) == m_Map.end()) { m_Map[_name] = std::make_shared<InputBind>(); }
    }

    void InputMapping::Remove(const std::string& _name)
    {
        // remove name from m_KeyToStrings
        for (auto it = m_KeyToStrings.begin(); it != m_KeyToStrings.end();)
        {
            auto& vec = it->second;
            vec.erase(std::remove(vec.begin(), vec.end(), _name), vec.end());

            if (vec.empty()) it = m_KeyToStrings.erase(it);
            else ++it;
        }

        m_Map.erase(_name);
        m_Axis1D.erase(_name);
        m_Axis2D.erase(_name);
    }
} // namespace MirageEngine::Core::Input
