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
        p_Type = LightType::DIRECTIONNAL;
        LOG(logTemp, LogDirectionalLight, "Directional light created");
    }

    DirectionalLight::DirectionalLight(GameObject* _owner)
		: Component(_owner)
    {
        p_Type = LightType::DIRECTIONNAL;
        LOG(logTemp, LogDirectionalLight, "Directional light created");
    }

    void DirectionalLight::AttachNew(std::shared_ptr<GameObject> _owner)
    {
        std::shared_ptr<DirectionalLight> compPtr = std::make_shared<DirectionalLight>(_owner.get());
		LightManager::Instance().AddDirectionalLight(compPtr);
        _owner->AddComponent<DirectionalLight>(compPtr);
    }

    void DirectionalLight::OnStart()
    {
    }

    entt::id_type DirectionalLight::GetType() const 
    {
        return entt::type_id<DirectionalLight>().hash();
    }
} // namespace MirageEngine
