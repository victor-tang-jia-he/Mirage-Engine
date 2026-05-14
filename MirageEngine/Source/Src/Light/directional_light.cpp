#include <Light/directional_light.h>

#include <Light/light_manager.h>
#include <GameObject/game_object.h>
#include <entt/entt.hpp>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogDirectionalLight)

namespace MirageEngine
{
    DirectionalLight::DirectionalLight()
    {
        m_Type = LightType::Directional;
    }

    DirectionalLight::DirectionalLight(GameObject* _owner)
		: Component(_owner)
    {
        m_Type = LightType::Directional;
    }

    void DirectionalLight::AttachNew(std::shared_ptr<GameObject> _owner)
    {
        std::shared_ptr<DirectionalLight> compPtr = std::make_shared<DirectionalLight>(_owner.get());
        _owner->AddComponent<DirectionalLight>(compPtr);
    }

    void DirectionalLight::OnStart()
    {
        LOG(logTemp, LogDirectionalLight, "Directional light created");
        LightManager::Instance().AddDirectionalLight(shared_from_this());
    }

    entt::id_type DirectionalLight::GetType() const 
    {
        return entt::type_id<DirectionalLight>().hash();
    }
} // namespace MirageEngine
